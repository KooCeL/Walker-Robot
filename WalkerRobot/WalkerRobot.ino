/*
 * Walker Robot: joystick-controlled snow walker
 * Imperial College CGCU Makeathon, March 2026 (2nd place)
 *
 * Two DC motors drive paddles that push the robot through snow.
 * An analog thumb joystick picks the direction:
 *
 *   Joystick   | Motor 1        | Motor 2
 *   -----------+----------------+----------------
 *   Forward    | clockwise      | clockwise
 *   Backward   | anticlockwise  | anticlockwise
 *   Right      | clockwise      | anticlockwise
 *   Left       | anticlockwise  | clockwise
 *   Centre     | stop           | stop
 *
 * Hardware: Arduino Uno, analog joystick module, L298N dual H-bridge driver.
 * Change the pin numbers below to match your wiring.
 */

// ---------------------------------------------------------------------------
// Pin assignments
// ---------------------------------------------------------------------------

// Joystick
const int JOY_X_PIN = A0;   // VRx: left / right
const int JOY_Y_PIN = A1;   // VRy: forward / backward

// Motor 1 (L298N channel A)
const int M1_EN  = 5;       // ENA: PWM speed
const int M1_IN1 = 7;       // IN1
const int M1_IN2 = 8;       // IN2

// Motor 2 (L298N channel B)
const int M2_EN  = 6;       // ENB: PWM speed
const int M2_IN1 = 9;       // IN3
const int M2_IN2 = 10;      // IN4

// ---------------------------------------------------------------------------
// Tuning
// ---------------------------------------------------------------------------

const int JOY_CENTER   = 512;   // ADC reading with the stick at rest
const int DEADZONE     = 150;   // ignore small stick movements around centre
const int MOTOR_SPEED  = 255;   // PWM duty for the paddles (0-255)

// Flip these if the robot goes the wrong way for a direction on the stick.
// (Many joystick modules read LOW when pushed "up".)
const bool INVERT_X = false;
const bool INVERT_Y = true;

// Set to true to print joystick readings and the chosen move to Serial.
const bool DEBUG = false;

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

enum Spin { STOP, CLOCKWISE, ANTICLOCKWISE };

enum Move { MOVE_STOP, MOVE_FORWARD, MOVE_BACKWARD, MOVE_LEFT, MOVE_RIGHT };

struct Motor {
  int en;
  int in1;
  int in2;
};

const Motor MOTOR_1 = { M1_EN, M1_IN1, M1_IN2 };
const Motor MOTOR_2 = { M2_EN, M2_IN1, M2_IN2 };

// ---------------------------------------------------------------------------
// Motor control
// ---------------------------------------------------------------------------

void setupMotor(const Motor &m) {
  pinMode(m.en, OUTPUT);
  pinMode(m.in1, OUTPUT);
  pinMode(m.in2, OUTPUT);
}

// "Clockwise" means IN1 HIGH / IN2 LOW. If a motor spins the other way,
// swap its two wires on the L298N output terminals.
void driveMotor(const Motor &m, Spin spin, int speed) {
  switch (spin) {
    case CLOCKWISE:
      digitalWrite(m.in1, HIGH);
      digitalWrite(m.in2, LOW);
      analogWrite(m.en, speed);
      break;
    case ANTICLOCKWISE:
      digitalWrite(m.in1, LOW);
      digitalWrite(m.in2, HIGH);
      analogWrite(m.en, speed);
      break;
    default:
      digitalWrite(m.in1, LOW);
      digitalWrite(m.in2, LOW);
      analogWrite(m.en, 0);
      break;
  }
}

void drive(Spin motor1, Spin motor2) {
  driveMotor(MOTOR_1, motor1, MOTOR_SPEED);
  driveMotor(MOTOR_2, motor2, MOTOR_SPEED);
}

void forward()  { drive(CLOCKWISE,     CLOCKWISE);     }
void backward() { drive(ANTICLOCKWISE, ANTICLOCKWISE); }
void right()    { drive(CLOCKWISE,     ANTICLOCKWISE); }
void left()     { drive(ANTICLOCKWISE, CLOCKWISE);     }
void stopAll()  { drive(STOP,          STOP);          }

// ---------------------------------------------------------------------------
// Joystick
// ---------------------------------------------------------------------------

// Returns the stick offset from centre, roughly -512 to +511.
// Positive X = right, positive Y = forward.
int readAxis(int pin, bool invert) {
  int offset = analogRead(pin) - JOY_CENTER;
  return invert ? -offset : offset;
}

// Picks one of the five moves from the stick position. Whichever axis is
// pushed further wins, so diagonals resolve to the stronger direction.
Move readMove() {
  int x = readAxis(JOY_X_PIN, INVERT_X);
  int y = readAxis(JOY_Y_PIN, INVERT_Y);

  if (DEBUG) {
    Serial.print("x=");
    Serial.print(x);
    Serial.print(" y=");
    Serial.print(y);
    Serial.print("  ");
  }

  if (abs(x) < DEADZONE && abs(y) < DEADZONE) {
    return MOVE_STOP;
  }

  if (abs(y) >= abs(x)) {
    return (y > 0) ? MOVE_FORWARD : MOVE_BACKWARD;
  }
  return (x > 0) ? MOVE_RIGHT : MOVE_LEFT;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

void setup() {
  setupMotor(MOTOR_1);
  setupMotor(MOTOR_2);
  stopAll();

  if (DEBUG) {
    Serial.begin(9600);
  }
}

void loop() {
  Move move = readMove();

  switch (move) {
    case MOVE_FORWARD:  forward();  break;
    case MOVE_BACKWARD: backward(); break;
    case MOVE_RIGHT:    right();    break;
    case MOVE_LEFT:     left();     break;
    default:            stopAll();  break;
  }

  if (DEBUG) {
    const char *names[] = { "STOP", "FORWARD", "BACKWARD", "LEFT", "RIGHT" };
    Serial.println(names[move]);
  }

  delay(20);
}
