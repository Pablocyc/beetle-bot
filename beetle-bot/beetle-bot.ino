/******************************************************************
*  Programa para el control de un BEETLE-BOT (carrito con motores *
*  paso a paso y teclado programable), para la escuela MakerHouse.*
*                                                                 *
*  Teclado de 4 botones que usan divisor de voltaje               *
*  Motores paso a paso con driver ULN2003APG.                     *
*  Arduino-Nano.                                                  *
*                                                                 *
*    Author:   Pablo Cardozo                                      *
*    Company:  Electric Eye                                       *
*    Position: Frontend Developer                                 *
*                                                                 *
*******************************************************************
*                      PIN-OUT                                    *
*    Motor-Left: (beetle-bot delante)                             *
*     IN1: D2                                                     *
*     IN2: D3                                                     *
*     IN3: D4                                                     *
*     IN4: D5                                                     *
*    Motor-Right: (beetle-bot delante)                            *
*     IN1: D9                                                     *
*     IN2: D8                                                     *
*     IN3: D7                                                     *
*     IN4: D6                                                     *
*    Buzzer: D11                                                  *
*                                                                 *
*******************************************************************
*                      PIN-IN                                     *
*     Teclado:   A0                                               *
*                                                                 *
*                                                                 *
******************************************************************/

// Libraries
#include <EEPROM.h>

// Pin-out
int8_t pin_left[] = {2 , 3, 4, 5};
int8_t pin_right[] = {6, 7, 8, 9};
#define pin_buzzer  11
// Pin-in
#define   pin_key     A0
#define   btn_right   0
#define   btn_up      1
#define   btn_down    2
#define   btn_left    3
#define   btn_enter   4
#define   btn_none    5

// Variables Global
int addr = 0;
int time_buttons = 700;
int read_key;
int time_clear_sequence = 3000;
unsigned long currentTime;
bool flag_enable_buttons;
bool flag_new_sequence;
// Variables Tones
int tone_buttons = 3800;
int duration_buttons = 100;
int tone_enter = 3300;
int duration_on_off = 100;
int repeat_on_off = 5;
int tone_run = 3000;
int duration_run = 200;
int tone_cancel = 4000;
int duration_cancel = 1000;
// Variables keypad
char up    = 'u';
char right = 'r';
char down  = 'd';
char left  = 'l';
char enter = 'e';
// Variables stepping motors
int number_steps_turn = 150;
int number_steps_motor = 400;



void setup() {
  for(int8_t i=0; i<sizeof(pin_left); i++) {
    pinMode(pin_left[i], OUTPUT);
    pinMode(pin_right[i], OUTPUT);
  }
  Off();
  // Serial.begin(115200);
  // Serial.println("BEETLE-BOT");

  EEPROM.write(0,addr);
}

void loop() {
  // while(Serial.available()) {
  //   int steps = Serial.parseInt();
  //   int value = Serial.read();
  //   if(value == 't')  number_steps_turn = steps;
  //   if(value == 's')  number_steps_motor = steps;
  // }

  if(flag_enable_buttons)
    read_key = ReadButtons();

  if(read_key != btn_none) {
    ActionKey();
  }

  if(millis() > currentTime + time_buttons)
    flag_enable_buttons = true;
}


void ActionKey() {
  if(read_key == btn_enter) {
    int cont = 0;
    while(ReadButtons() == btn_enter) {
      cont++;
      delay(1);
      if(cont > time_clear_sequence) {
        tone(pin_buzzer, tone_cancel, duration_cancel);
        Clear();
      }
    }
    Enter();
  }
  else {
    if(flag_new_sequence)
      Clear();
    tone(pin_buzzer, tone_buttons, duration_buttons);
    addr++;
    EEPROM.write(0, addr);
  }
  if(read_key == btn_right)
    EEPROM.write(addr, right);
  if(read_key == btn_up)
    EEPROM.write(addr, up);
  if(read_key == btn_down)
    EEPROM.write(addr, down);
  if(read_key == btn_left)
    EEPROM.write(addr, left);

  read_key = btn_none;
}

void Enter() {
  if(EEPROM.read(0) > 0) {
    flag_new_sequence = true;
    int cont = EEPROM.read(0);
    for(int i=1; i<=cont; i++) {
      tone(pin_buzzer, tone_run, duration_run);
      int value = EEPROM.read(i);
      if(value == up)
        Forward(number_steps_motor);
      if(value == right)
        Right(number_steps_turn);
      if(value == left)
        Left(number_steps_turn);
      if(value == down)
        Back(number_steps_motor);
    }

    for(int i=0; i<repeat_on_off; i++) {
      tone(pin_buzzer, tone_enter, duration_on_off);
      delay(duration_on_off * 2);
    }
  }
}
void Clear() {
  addr = 0;
  EEPROM.write(0, addr);
  flag_new_sequence = false;
}


void Right(int cont) {
  while(cont>0) {
    for(int i=0; i<4; i++) {
      Off();
      digitalWrite(pin_right[i], HIGH);
      digitalWrite(pin_left[3-i], HIGH);
      delay(2);
    }
    cont--;
  }
  Off();
}
void Left(int cont) {
  while(cont>0) {
    for(int i=0; i<4; i++) {
      Off();
      digitalWrite(pin_left[i], HIGH);
      digitalWrite(pin_right[3-i], HIGH);
      delay(2);
    }
    cont--;
  }
  Off();
}
void Back(int cont) {
  while(cont>0) {
    for(int i=0; i<4; i++) {
      Off();
      digitalWrite(pin_right[i], HIGH);
      digitalWrite(pin_left[i], HIGH);
      delay(2);
    }
    cont--;
  }
  Off();
}
void Forward(int cont) {
  while(cont>0) {
    for(int i=3; i>=0; i--) {
      Off();
      digitalWrite(pin_right[i], HIGH);
      digitalWrite(pin_left[i], HIGH);
      delay(2);
    }
    cont--;
  }
  Off();
}
void Off() {
  for(int8_t i=0; i<4; i++) {
    digitalWrite(pin_left[i], LOW);
    digitalWrite(pin_right[i], LOW);
  }
}

int ReadButtons() {
  int adc_key = analogRead(pin_key);
  if(adc_key < 800) {
    currentTime = millis();
    flag_enable_buttons = false;
  }
  if(adc_key < 50)   return btn_right;
  if(adc_key < 200)  return btn_up;
  if(adc_key < 380)  return btn_down;
  if(adc_key < 555)  return btn_left;
  if(adc_key < 790)  return btn_enter;
  return btn_none;
}