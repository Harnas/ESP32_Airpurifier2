
#include <Arduino.h>
// #include <WiFi.h>

#include "AiEsp32RotaryEncoder.h"
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <config.h>

#include <AirCleanerController.hpp>
#include <DustSensor.hpp>
#include <EasyButton.h>
#include <Fan.hpp>
#include <Screen.hpp>
#include <Sensors.hpp>
#include <Settings.hpp>
#include <UIController.hpp>

// WiFiServer server(80);

EasyButton encoderButton(ROTARY_ENCODER_BUTTON_PIN);
EasyButton boardButton(BOARD_BUTTON_PIN);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/OLED_CLK, /* data=*/OLED_SDA); // ESP32 Thing, HW I2C with pin remapping
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1);

Settings settings;
Adafruit_BME280 bme;
DustSensor dustSensor(Dust_sensor_LED_pin, Dust_sensor_IN_pin, Dust_sensor_V_ref_pin);
Sensors sensors(&bme, &dustSensor);
Fan fan(FAN_EN_pin, Fan_PWM_pin, 0);
Screen screen(&u8g2);
AirCleanerController controller(&settings, &sensors, &fan, &rotaryEncoder);
UIController uiController(&screen, &controller);

// End of constructor list
TaskHandle_t SensorTaskHandle;
TaskHandle_t UITaskHandle;
SemaphoreHandle_t UIRefreshSemaphore = xSemaphoreCreateBinary();

void encoder_read()
{
  int16_t encoderDelta = rotaryEncoder.encoderChanged();

  if (encoderDelta != 0)
  {
    uiController.knobRotation(encoderDelta);
  }
}

void encoderButtonPressed(void)
{
  uiController.knobPress();
  xSemaphoreGive(UIRefreshSemaphore);
}
void boardButtonPressed(void)
{
  uiController.resetInteracionTimer(Button);
  xSemaphoreGive(UIRefreshSemaphore);

  digitalWrite(LED_BUILTIN, LOW);
}
void boardButtonPressedLong(void)
{
  uiController.resetInteracionTimer(Button);
  xSemaphoreGive(UIRefreshSemaphore);
}

/*
///////////////////////////////////////////////////////////////
Tasks
*/

void UITask(void *parameter)
{
  uiController.init();
  for (;;)
  {
    if (xSemaphoreTake(UIRefreshSemaphore, (TickType_t)500) == pdTRUE)
    {
    }
    uiController.renderScreen();
  }
}

void sensorTask(void *parameter)
{
  int last_full_refresh_time = 0;
  for (;;)
  {
    delay(SENSORTASK_SMALL_REFRESH_TIME);
    last_full_refresh_time += SENSORTASK_SMALL_REFRESH_TIME;

    if (last_full_refresh_time > SENSORTASK_FULL_REFRESH_TIME)
    {
      last_full_refresh_time = 0;

      controller.read_sensors();
    }

    encoderButton.read();
    boardButton.read();
    encoder_read();

    uiController.incActionTimer(SENSORTASK_SMALL_REFRESH_TIME);
  }
}

void setup()
{
  Serial.begin(9600);
  settings.begin();
  settings.load();
  delay(200);

  Wire.begin(BME_SDA, BME_SCL);
  bme.begin(0x58, &Wire); // 0x58 for BMP180

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(ROTARY_ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ROTARY_ENCODER_B_PIN, INPUT_PULLUP);

  encoderButton.begin();
  encoderButton.onPressed(encoderButtonPressed);

  boardButton.begin();
  boardButton.onPressed(boardButtonPressed);
  boardButton.onPressedFor(2000, boardButtonPressedLong);

  rotaryEncoder.begin();
  rotaryEncoder.setup([] { rotaryEncoder.readEncoder_ISR(); });
  rotaryEncoder.setBoundaries(-2000, 2000, false);
  rotaryEncoder.reset(0);

  xTaskCreate(
      sensorTask,   /* Task function. */
      "sensorTask", /* String with name of task. */
      20000,        /* Stack size in bytes. */
      NULL,         /* Parameter passed as input of the task */
      2,            /* Priority of the task. */
      &SensorTaskHandle);

  xTaskCreate(
      UITask,   /* Task function. */
      "UITask", /* String with name of task. */
      20000,    /* Stack size in bytes. */
      NULL,     /* Parameter passed as input of the task */
      1,        /* Priority of the task. */
      &UITaskHandle);

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  vTaskSuspend(NULL);
  delay(10000);
}
