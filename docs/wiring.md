# Wiring & Power Guide

## Power wiring — important

**Do not connect TP4056 OUT+ directly to VIN on a typical Arduino Uno/Nano.** The protected TP4056 output follows the battery voltage (roughly 3.0–4.2 V), which is generally too low for VIN. Use a regulated 5 V boost converter for a typical 5 V Arduino board, and check your exact board's documentation before soldering.

### Battery and charger
- Battery positive (+) → TP4056 **B+**
- Battery negative (−) → TP4056 **B−**
- The USB-C cable plugs into the charging module's USB-C port; no separate USB wiring is needed.

### Protected output, switch, and boost converter
- TP4056 **OUT+** → one switch terminal
- Other switch terminal → boost converter **IN+**
- TP4056 **OUT−** → boost converter **IN−**
- Adjust/check the boost converter output to **5.0 V** *before* connecting it to the Arduino.
- Boost converter **OUT+** (regulated 5 V) → Arduino **5V** pin (only if your board supports 5 V input on that pin)
- Boost converter **OUT−** → Arduino **GND**

All modules must share ground: TP4056 OUT−, boost IN−/OUT−, Arduino GND, OLED GND, and joystick GND.

If your Arduino is not a 5 V Nano/Uno, do not assume these instructions apply — follow your board's specified input voltage. The TP4056 is a charger/protection board, not a regulated 5 V supply. Many TP4056 boards do not provide proper load-sharing while charging; unless your module supports it, **turn the game station off while charging.**

## Signal wiring (typical Arduino Nano/Uno)

| Part | Pin | Arduino |
|------|-----|---------|
| OLED | VCC | 5V (or the module's documented supply voltage) |
| OLED | GND | GND |
| OLED | SDA | A4 |
| OLED | SCL | A5 |
| Joystick | VRx / X | A0 |
| Joystick | VRy / Y | A1 |
| Joystick | SW | D2 |
| Joystick | VCC | 5V (typical; check module) |
| Joystick | GND | GND |

A3 is used only for `randomSeed(analogRead(A3))` — leave it unconnected.

## Before soldering and first power-up
- Verify battery polarity and the exact labels printed on the charging board. Don't rely on pad position alone.
- Insulate exposed battery leads and solder joints; keep the battery from being pinched or punctured.
- With the battery disconnected, check for solder bridges and shorts between positive and ground.
- Set and measure the boost converter output at 5.0 V before connecting it to the Arduino.
- Use a protected, undamaged single-cell battery and the correct charge-current setting for it.
- Don't leave the battery charging unattended, or charge it inside the enclosure unless the design is made for safe charging.
