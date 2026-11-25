# Arduino Game Station

A compact handheld game console built from an Arduino, a 128×64 SSD1306 OLED, and an analog joystick. It runs three simple games: **Snake**, **Dodge**, and **Shooter**. Powered by a single-cell 3.7 V LiPo/Li-ion battery with USB-C charging and an on/off switch.

## Controls
- **Joystick up/down** – move through the menu
- **Joystick button** – select a game (and fire in Shooter)
- **Joystick left/right** – steer / move

## Parts
- Arduino-compatible board (sketch uses A0, A1, A3, D2, and I²C — confirm your board and its power-pin requirements)
- 128×64 SSD1306 I²C OLED (address `0x3C`)
- Analog joystick module (X, Y, push button)
- Single-cell 3.7 V LiPo/Li-ion battery
- USB-C TP4056 charging module with battery protection (B+, B−, OUT+, OUT− pads)
- SPST on/off switch
- Regulated 5 V boost converter (required for a typical 5 V Nano/Uno)
- Wire, solder, heat-shrink, enclosure

## Wiring
See [docs/wiring.md](docs/wiring.md). **Read the power section first — never connect TP4056 OUT+ directly to VIN.**

## Software setup
1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install libraries via *Library Manager*: **Adafruit GFX Library** and **Adafruit SSD1306**.
3. Open `game_station/game_station.ino`.
4. Select your board and port, then upload.

## Safety
Lithium cells can be dangerous if shorted, punctured, or charged improperly. Use a protected cell, insulate all connections, and don't charge unattended.

## License
MIT — see [LICENSE](LICENSE).
