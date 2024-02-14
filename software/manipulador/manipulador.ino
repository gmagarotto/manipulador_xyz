#include <Arduino.h>
#include <Servo.h>


// Pinos da placa
constexpr auto PIN_CLAW = PIN_PB5;
constexpr auto PIN_BUTTON = PIN_PA6;

constexpr auto PIN_LED_BLUE = PIN_PA4;

constexpr auto PIN_XYE_MOT_ENABLE = PIN_PD6;
constexpr auto PIN_Z_MOT_ENABLE = PIN_PA5;

constexpr auto PIN_XMOT_STEP = PIN_PD7;
constexpr auto PIN_XMOT_DIR = PIN_PC5;

constexpr auto PIN_YMOT_STEP = PIN_PC6;
constexpr auto PIN_YMOT_DIR = PIN_PC7;

constexpr auto PIN_ZMOT_STEP = PIN_PB3;
constexpr auto PIN_ZMOT_DIR = PIN_PB2;

constexpr auto PIN_EMOT_STEP = PIN_PB1;
constexpr auto PIN_EMOT_DIR = PIN_PB0;

// Contantes
constexpr auto XMOT_STEP_PER_MM = 80;
constexpr auto YMOT_STEP_PER_MM = 80;
constexpr auto ZMOT_STEP_PER_MM = 400;

constexpr auto CLAW_OPENED = 80;
constexpr auto CLAW_CLOSED = 0;

// Axis
enum Axis {
  X,
  Y,
  Z
};

// constantes por motor
constexpr int motor_step_pin[3] = {PIN_XMOT_STEP, PIN_YMOT_STEP, PIN_ZMOT_STEP};
constexpr int motor_dir_pin[3] = {PIN_XMOT_DIR, PIN_YMOT_DIR, PIN_ZMOT_DIR};
constexpr int motor_relacao_mecanica[3] = {XMOT_STEP_PER_MM, YMOT_STEP_PER_MM, ZMOT_STEP_PER_MM};
constexpr int motor_direction[3] = {HIGH, LOW, HIGH};

// Posição atual do motor
unsigned long motor_pos[3] = {0, 0, 0};

// estrutura de uma posição
struct Position {
  unsigned long x;
  unsigned long y;
};

// posição de cada caixa no palete
Position pallet_box[9] = {
  {161.3, 161.3},
  {125.55, 161.3},
  {89.8, 161.3},
  {161.3, 125.55},
  {125.55, 125.55},
  {89.8, 125.55},
  {161.3, 89.8},
  {125.55, 89.8},
  {89.8, 89.8},
};

// 
Position bottle = {0 , 0};
Servo myservo;

void enableMotors() {
  // habilita os motores
  digitalWrite(PIN_XYE_MOT_ENABLE, LOW);
  digitalWrite(PIN_Z_MOT_ENABLE, LOW);

  // zerar as posições
  motor_pos[X] = 0;
  motor_pos[Y] = 0;
  motor_pos[Z] = 0;
}

void disableMotors() {
  // desabilita os motores
  digitalWrite(PIN_XYE_MOT_ENABLE, HIGH);
  digitalWrite(PIN_Z_MOT_ENABLE, HIGH);
}

void moveAxisSteps(Axis axis, unsigned long posAbsSteps, long freqHz) {
  long totalSteps = posAbsSteps - motor_pos[axis];
  if(totalSteps > 0){
    digitalWrite(motor_dir_pin[axis], motor_direction[axis]);
  }else{
    digitalWrite(motor_dir_pin[axis], !motor_direction[axis]);
    totalSteps = totalSteps * -1;
  }
  delay(2);
  double d = 1000000.0/(2.0*(double)freqHz); 
  auto stepPin = motor_step_pin[axis];
  for (long i=0; i< totalSteps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(d);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(d);
  }
  motor_pos[axis] = posAbsSteps;
}

void moveAxis(Axis axis, unsigned long posAbs, unsigned long vel) { // mm, mm/s
  unsigned long posAbsSteps = posAbs * motor_relacao_mecanica[axis];
  long freqHz = vel * motor_relacao_mecanica[axis];
  moveAxisSteps(axis ,posAbsSteps, freqHz);
}

void move(Position pos, unsigned long vel) {
  moveAxis(Axis::Y, pos.y, vel);
  moveAxis(Axis::X, pos.x, vel);
}

void moveClaw(int pos) {
  myservo.write(pos);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_XYE_MOT_ENABLE, OUTPUT);
  pinMode(PIN_Z_MOT_ENABLE, OUTPUT);
  pinMode(PIN_XMOT_STEP, OUTPUT);
  pinMode(PIN_XMOT_DIR, OUTPUT);
  pinMode(PIN_YMOT_STEP, OUTPUT);
  pinMode(PIN_YMOT_DIR, OUTPUT);
  pinMode(PIN_ZMOT_STEP, OUTPUT);
  pinMode(PIN_ZMOT_DIR, OUTPUT);
  pinMode(PIN_EMOT_STEP, OUTPUT);
  pinMode(PIN_EMOT_DIR, OUTPUT);
  myservo.attach(PIN_CLAW);
  disableMotors();
}

void loop() {
  
  moveClaw(CLAW_OPENED);
  disableMotors();

  // aguarda botão
  while(digitalRead(PIN_BUTTON) == HIGH) {}

  // enable motors
  enableMotors();

  auto zvel = 16;

  for (int box = 0; box<9; box++) {
    // pega garrafa
    move(bottle, 100);            delay(1000);
    moveAxis(Axis::Z,   0, zvel); delay(1000);
    while(digitalRead(PIN_BUTTON) == HIGH) {} // aguarda botão
    moveClaw(CLAW_CLOSED);
    delay(1000);
    moveAxis(Axis::Z,  90, zvel); delay(1000);

    // guarda garrafa
    move(pallet_box[box], 100);   delay(1000);
    moveAxis(Axis::Z,  5, zvel);  delay(1000);
    moveClaw(CLAW_OPENED);
    moveAxis(Axis::Z, 90, zvel); delay(1000);
  }
  
  move(bottle, 100);
  moveAxis(Axis::Z, 0, zvel);

}
