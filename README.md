# Description
DIY air purifier. The electronics is based on ESP32 board. The first version of firmware was written using Micropython and published there https://github.com/Harnas/ESP32_Airpurifier. This version is written using VSCode with PlatformIO plugin in C++, Arduino and FreeRTOS.

Used elements:
```
- ESP32 board
- Arduino Uno prototype shield
- GP2Y1010AU0F
- BMP280
- HDC1080
- 0.91" OLED I2C screent
- rotary encoder
- 200mm 12V PC fan
```

Features:
- [x] Sensor readings
- [x] Fan speed control
- [x] Encoder knob
- [x] OLED screen UI
- [x] Settings stored in non-volatile memory
- [x] Power management - auto screen brightness and CPU clock control
- [ ] Captive portal
- [ ] Web control panel - LAN
- [ ] Web control panel - Internet webapp


More description on https://forbot.pl/forum/topic/15995-oczyszczacz-powietrza/
