# Walker Robot ❄️🤖

A joystick-controlled snow robot that uses two motor-driven paddles to push its way through snow.

**🥈 2nd place at the Imperial College CGCU Makeathon (March 2026)**

I built this robot in March 2026 for the Imperial College London CGCU (City & Guilds College Union) Makeathon, where it won 2nd place. It isn't a complicated machine, but it works: you push the joystick and the robot walks through the snow.

---

## How it works

The robot has two DC motors, each turning a paddle. An Arduino reads an analog thumb joystick and sets each motor's direction through an L298N H-bridge driver. Turning one paddle forward and the other backward makes the robot turn on the spot, like a tank.

| Joystick | Motor 1 | Motor 2 | Result |
|---|---|---|---|
| ⬆️ Forward | Clockwise | Clockwise | Walks forward |
| ⬇️ Backward | Anticlockwise | Anticlockwise | Walks backward |
| ➡️ Right | Clockwise | Anticlockwise | Turns right |
| ⬅️ Left | Anticlockwise | Clockwise | Turns left |
| ⏺️ Centre | Stop | Stop | Stays still |

A deadzone around the joystick's centre keeps the robot still when nobody is touching the stick. If the stick is pushed diagonally, whichever axis is pushed further wins.

```
            Joystick (X, Y)
                  │
                  ▼
        ┌───────────────────┐
        │    Arduino Uno    │   reads A0 / A1, picks a move
        └─────────┬─────────┘
                  │ IN1–IN4, ENA/ENB (PWM)
                  ▼
        ┌───────────────────┐
        │  L298N H-bridge   │
        └────┬─────────┬────┘
             ▼         ▼
         Motor 1    Motor 2
         (paddle)   (paddle)
```

## Repository layout

```
Walker-Robot/
├── WalkerRobot/
│   └── WalkerRobot.ino   # Arduino sketch
├── docs/
│   └── wiring.md         # Pin-by-pin wiring guide
└── README.md
```

## Hardware

- Arduino Uno (or compatible)
- Analog thumb joystick module (2-axis, e.g. KY-023)
- L298N dual H-bridge motor driver
- 2 × DC gear motors, each driving a paddle
- Battery pack for the motors (separate from the Arduino's USB power)
- Chassis, paddles and jumper wires

See [`docs/wiring.md`](docs/wiring.md) for the full wiring.

## Getting started

1. Wire everything up following [`docs/wiring.md`](docs/wiring.md).
2. Open `WalkerRobot/WalkerRobot.ino` in the [Arduino IDE](https://www.arduino.cc/en/software).
3. Select **Tools → Board → Arduino Uno** and the right port.
4. Click **Upload**.
5. Push the joystick to drive.

## Tuning

The settings are at the top of the sketch:

| Setting | Default | What it does |
|---|---|---|
| `JOY_CENTER` | `512` | Joystick reading at rest |
| `DEADZONE` | `150` | How far the stick must move before the robot reacts |
| `MOTOR_SPEED` | `255` | Paddle motor PWM speed (0–255) |
| `INVERT_X` / `INVERT_Y` | `false` / `true` | Flip an axis if left/right or forward/backward are reversed |
| `DEBUG` | `false` | Print joystick readings and moves to the Serial Monitor (9600 baud) |

**Motor spinning the wrong way?** Swap that motor's two wires on the L298N output terminals.

## Acknowledgements

Built for the [Imperial College CGCU](https://www.cgcu.net/) Makeathon, March 2026.
