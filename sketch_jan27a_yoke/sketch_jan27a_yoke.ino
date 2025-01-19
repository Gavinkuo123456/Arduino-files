#include <Joystick.h>
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  9, 0,                  // Button Count, Hat Switch Count
  true, true, true,     // X and Y, but no Z Axis
  true, false, false,   // No Rx, Ry, or Rz
  true, true,          // No rudder or throttle
  false, false, false);

  bool AP = 0;
  bool APAlt = 0;
  
  
void setup() {
  pinMode(13,INPUT);
  pinMode(12,INPUT);
  pinMode(11,INPUT);
  pinMode(10,INPUT);
  pinMode(9,INPUT);
  pinMode(8,INPUT);
  pinMode(7,INPUT);
  pinMode(6,INPUT);

  if(digitalRead(9) == HIGH){
    AP == 1;
  }else{
    AP == 0;
  }
  if(digitalRead(8) == HIGH){
    APAlt == 1;
  }else{
    APAlt == 0;
  }
  
  Joystick.begin();
  Joystick.setXAxisRange(0, 1023);
  Joystick.setYAxisRange(0, 1023);
  Joystick.setZAxisRange(0, 1023);
  Joystick.setThrottleRange(0, 1023);
  Joystick.setRudderRange(0, 1023);
}

void loop() {
  Joystick.setXAxis(analogRead(A1));
  Joystick.setYAxis(analogRead(A2));
  Joystick.setZAxis(analogRead(A3));
  Joystick.setThrottle(analogRead(A0));
  Joystick.setRudder(analogRead(A4));
  Joystick.setRxAxis(analogRead(10));
  Joystick.setButton(2, digitalRead(11));
  Joystick.setButton(3, !digitalRead(11));
  Joystick.setButton(0, digitalRead(13));
  Joystick.setButton(1, digitalRead(12));
  Joystick.setButton(8, digitalRead(6));

  if(AP != digitalRead(9)){
    if(digitalRead(9) == HIGH){
     Joystick.pressButton(4);
     delay(50);
     Joystick.releaseButton(4);
     AP = 1;
    }else{
      Joystick.pressButton(5);
      delay(50);
     Joystick.releaseButton(5);
     AP = 0;
    }
  }
  if(APAlt != digitalRead(8)){
    if(digitalRead(8) == HIGH){
     Joystick.pressButton(6);
     delay(50);
     Joystick.releaseButton(6);
     APAlt = 1;
    }else{
      Joystick.pressButton(7);
      delay(50);
     Joystick.releaseButton(7);
     APAlt = 0;
    }
  }

}
