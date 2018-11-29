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
*     IN1: D6                                                     *
*     IN2: D7                                                     *
*     IN3: D8                                                     *
*     IN4: D9                                                     *
*    Buzzer: D13                                                  *
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
// Pin-in
#define   pin_key   A0


// Variables
bool flag_right;
bool flag_enable;

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
  if(Serial.available()) {
    char value = Serial.read();
    delay(2);
    Serial.println(value);
    if(value == 'a')
      flag_right = true;
    if(value == 'b')
      flag_right = false;
    flag_enable = true;
    if(value == 'x')
      Off(), flag_enable = false;
  }

  if(flag_enable) {
    if(flag_right)
      Right();
    else
      Left();
  }

}

void Right() {
  for(int i=0; i<4; i++) {
    Off();
    digitalWrite(pin_left[i], HIGH);
    delay(2);
  }
}
void Left() {
  for(int i=3; i>=0; i--) {
    Off();
    digitalWrite(pin_left[i], HIGH);
    delay(2);
  }
}

void Off() {
  for(int8_t i=0; i<sizeof(pin_left); i++) {
    digitalWrite(pin_left[i], LOW);
    digitalWrite(pin_right[i], LOW);
  }
}