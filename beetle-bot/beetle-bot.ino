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
int8_t pin_left[] = {2, 3, 4, 5};
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


// Variables
unsigned long currentTime;
bool flag_right;
bool flag_enable;
bool flag_tone;

void setup() {
  for(int8_t i=0; i<sizeof(pin_left); i++) {
    pinMode(pin_left[i], OUTPUT);
    pinMode(pin_right[i], OUTPUT);
  }
  Off();
  Serial.begin(115200);
  Serial.println("BEETLE-BOT");
}

void loop() {
  while(Serial.available()>0) {
    int number_steps = Serial.parseInt();
    int value = Serial.read();
    if(value == 'l')  Left(number_steps);
    if(value == 'r')  Right(number_steps);
    if(value == 'b')  Back(number_steps);
    if(value == 'f')  Forward(number_steps);
  }
  int read_key = ReadButtons();
  if(read_key != btn_none) {
    if(read_key == btn_right)
      Serial.println("BTN-RIGHT");
    if(read_key == btn_up)
      Serial.println("BTN-UP");
    if(read_key == btn_down)
      Serial.println("BTN-DOWN");
    if(read_key == btn_left)
      Serial.println("BTN-LEFT");
    if(read_key == btn_enter)
      Serial.println("BTN-ENTER");
  }
}


void Right(int cont) {
  Serial.println("Right");
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
  Serial.println("Left");
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
  Serial.println("Back");
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
  Serial.println("Forward");
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
  if(adc_key < 1000) {
    Serial.print("adc_key: ");
    Serial.println(adc_key);
  }
  delay(50);
  if(adc_key < 50)   return btn_right;
  if(adc_key < 250)  return btn_up;
  if(adc_key < 380)  return btn_down;
  if(adc_key < 555)  return btn_left;
  if(adc_key < 800)  return btn_enter;
  return btn_none;
}