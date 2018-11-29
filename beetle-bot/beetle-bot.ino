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
#define   pin_key   A0


// Variables
unsigned int cont = 0;
int frequency = 3000;
int time_on;
int time_off;
int repeat;
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
  while(Serial.available()) {
    int f = Serial.parseInt();
    int on = Serial.parseInt();
    int off = Serial.parseInt();
    int n = Serial.parseInt();

    if(Serial.read() == 't') {
      frequency = f;
      time_on = on;
      time_off = off;
      repeat = n;
      flag_tone = true;
    }
  }

  if(flag_tone) {
    if(millis() > currentTime + time_on + time_off) {
      currentTime = millis();
      tone(pin_buzzer, frequency, time_on);
      repeat--;
      if(repeat <= 0)
        flag_tone = false;
    }
  }

}

void Right() {
  for(int i=0; i<4; i++) {
    Off();
    digitalWrite(pin_left[i], HIGH);
    delay(2);
  }
  cont++;
}
void Left() {
  for(int i=3; i>=0; i--) {
    Off();
    digitalWrite(pin_left[i], HIGH);
    delay(2);
  }
  cont++;
}

void Off() {
  for(int8_t i=0; i<sizeof(pin_left); i++) {
    digitalWrite(pin_left[i], LOW);
    digitalWrite(pin_right[i], LOW);
  }
  if(!flag_enable)
    Serial.println(cont), cont = 0;
}