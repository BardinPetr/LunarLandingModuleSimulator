#define PITCH 1
#define ROLL  2
#define THR   3
#define YAW   4

#define MIN   1000
#define MAX   2000

#define SERVOMIN 0
#define SERVOMAX 0


#include "alexmos.h"
#include <Servo.h>
//#include "Servo.h"

Servo chan[8];
Servo servo;

String input = "";
bool stringComplete = false;

int a = 1500;
int b = 1500;
int c = 1000;
int d = 1500;
int e = 0;
int f = 0;
int g = 0;
int h = 0;

void setup() {
  chan[ ROLL ].attach(3);
  chan[ PITCH].attach(9);
  chan[ THR  ].attach(10);
  chan[ YAW  ].attach(11);

  servo.attach(5);

  Serial.begin(57600);
  input.reserve(200);

  gimbal_setup();
}

void loop() {
  /* Protocol
     radio (int) - 1000--2000
     gimbal(int) - 0--40
     servo (int) - 0--1
     radio1@radio2@radio3@radio4@gimpalROLL@gimpalPITCH@gimpalYAW@servo

     Example:
     1500@1500@1000@1500@0@0@0@0
     1500@1500@1000@1500@20@-20@20@0
  */
  if (stringComplete) {
    if (input.startsWith("!")) {
      input = input.substring(1, 21);
      Serial.println(input);
      int _a = getValue(input, '@', 0).toInt();
      int _b = getValue(input, '@', 1).toInt();
      int _c = getValue(input, '@', 2).toInt();
      int _d = getValue(input, '@', 3).toInt();

      int _e = getValue(input, '@', 4).toInt();
      int _f = getValue(input, '@', 5).toInt();
      int _g = getValue(input, '@', 6).toInt();

      int _h = getValue(input, '@', 7).toInt();

      if (_a >= 1000 && _a <= 2000 &&
          _b >= 1000 && _b <= 2000 &&
          _c >= 1000 && _c <= 2000 &&
          _d >= 1000 && _d <= 2000 &&
          _e >= 0 && _e <= 30 &&
          _f >= 0 && _f <= 30 &&
          _g >= 0 && _g <= 30) {
        a = _a;
        b = _b;
        c = _c;
        d = _d;
        e = _e;
        f = _f;
        g = _g;
        h = _h;
      }
    }
    input = "";
    stringComplete = false;
  }
  else {
    setRC(PITCH,  a);
    setRC(ROLL,   b);
    setRC(THR,    c);
    setRC(YAW,    d);

    set_spd(ROLL, e);
    set_spd(PITCH, f);
    set_spd(YAW, g);

    servo.write((h ? SERVOMIN : SERVOMAX));
    delay(10);
  }
}

void arm() {
  setRC(YAW, 2000);
  setRC(THR, 1000);
  delay(4000);
  setRC(YAW, 1500);
  setRC(THR, 1500);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setRC(int id, int f) {
  chan[id].writeMicroseconds(f);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    input += inChar;
    if (inChar == '\r') {
      stringComplete = true;
    }
  }
}
