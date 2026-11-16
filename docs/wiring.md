# Wiring

These pin numbers match the defaults in `WalkerRobot/WalkerRobot.ino`. If you wire it differently, change the constants at the top of the sketch.

## Joystick → Arduino

| Joystick pin | Arduino pin |
|---|---|
| GND | GND |
| +5V | 5V |
| VRx (left/right) | A0 |
| VRy (forward/backward) | A1 |
| SW (button) | not used |

## L298N → Arduino

| L298N pin | Arduino pin | Purpose |
|---|---|---|
| ENA | D5 (PWM) | Motor 1 speed |
| IN1 | D7 | Motor 1 direction |
| IN2 | D8 | Motor 1 direction |
| ENB | D6 (PWM) | Motor 2 speed |
| IN3 | D9 | Motor 2 direction |
| IN4 | D10 | Motor 2 direction |
| GND | GND | **Common ground (required)** |

Take the jumper caps off ENA and ENB so the Arduino can control speed with PWM.

## Motors and power

| L298N terminal | Connect to |
|---|---|
| OUT1 / OUT2 | Motor 1 (left paddle) |
| OUT3 / OUT4 | Motor 2 (right paddle) |
| 12V | Motor battery + |
| GND | Motor battery − **and** Arduino GND |

## Notes

- **Shared ground:** the Arduino, L298N and battery must share a ground, or the motors won't respond properly.
- **Direction:** in the code, "clockwise" means IN1 HIGH / IN2 LOW. If a paddle turns the wrong way, swap that motor's two output wires.
- **Power:** don't run the motors from the Arduino's 5V pin. Use a separate battery pack for the L298N.
