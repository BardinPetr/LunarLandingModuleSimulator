#define PITCH 1
#define ROLL  2
#define THR   3
#define YAW   4

#define MIN   1000
#define MAX   2000

#include <Servo.h>

Servo chan[8];

String input = "";
bool stringComplete = false;

int a = 1500;
int b = 1500;
int c = 1000;
int d = 1500;

void setup() {
  chan[ ROLL ].attach(3);
  chan[ PITCH].attach(9);
  chan[ THR  ].attach(10);
  chan[ YAW  ].attach(11);

  Serial.begin(57600);
  input.reserve(200);

  //TCCR2B = (TCCR2B & 0b11111000) | 0x07;
  //analogWrite(11, 255);

  //while(1){}
}

void loop() {
  if (stringComplete) {
    if (input.startsWith("!")) {
      input = input.substring(1, 21);
      Serial.println(input);
      int _a = getValue(input, '@', 0).toInt();
      int _b = getValue(input, '@', 1).toInt();
      int _c = getValue(input, '@', 2).toInt();
      int _d = getValue(input, '@', 3).toInt();
      //Serial.print(_a);
      //Serial.print(" ");
      //Serial.print(_b);
      //Serial.print(" ");
      //Serial.print(_c);
      //Serial.print(" ");
      //Serial.println(_d);
      if (_a >= 1000 && _a <= 2000 && _b >= 1000 && _b <= 2000 && _c >= 1000 && _c <= 2000 && _d >= 1000 && _d <= 2000) {
        a = _a;
        b = _b;
        c = _c;
        d = _d;
      }
    }

    //Serial.print(a);
    //Serial.print(" ");
    //Serial.print(b);
    //Serial.print(" ");
    //Serial.print(c);
    //Serial.print(" ");
    //Serial.println(d);
    //Serial.println();

    input = "";
    stringComplete = false;
  }
  else {
    setRC(PITCH,  a);
    setRC(ROLL,   b);
    setRC(THR,    c);
    setRC(YAW,    d);
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

void p() {
  //Serial.println("#");
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
