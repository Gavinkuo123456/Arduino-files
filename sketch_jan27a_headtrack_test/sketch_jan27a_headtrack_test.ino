#include <Joystick.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include "Keyboard.h"
#include "Mouse.h"
MPU6050 mpu(Wire);
unsigned long timer = 0;
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  3, 0,                  // Button Count, Hat Switch Count
  true, true, true,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  true, false,          // No rudder or throttle
  false, false, false);
float preX = 0;
float preY = 0;
float mouseX = 0;
float mouseY = 0;


void setup() {
  pinMode(13,INPUT);
  pinMode(12,INPUT);
  pinMode(11,INPUT);
  pinMode(10,INPUT);
  Wire.begin();
  mpu.begin();

  delay(1000);
  mpu.calcOffsets();

  Joystick.begin();
  Joystick.setXAxisRange(-35, 35);
  Joystick.setYAxisRange(-20, 20);
  Joystick.setZAxisRange(0, 1023);
  Mouse.begin();
  Keyboard.begin();
}

void loop() {
  //Joystick.setButton(0, digitalRead(13));
  Joystick.setButton(1, digitalRead(12));
  Joystick.setButton(2, digitalRead(11));
  Joystick.setZAxis(analogRead(10));
if(digitalRead(13)){
  mpu.update();
 if ((millis() - timer) > 2) { // print data every 10ms
     int X = mpu.getAngleX();
     int Y = mpu.getAngleY();
     int Z = mpu.getAngleZ();

     preX = mouseX;
     preY = mouseY;

     mouseX = map(mpu.getAngleX(),-35,35 ,0,1280);
     mouseY = map(mpu.getAngleY(),-20,20,0,720);
     Mouse.move(mouseX-preX,mouseY-preY);
     Joystick.setXAxis(mpu.getAngleX());
     Joystick.setYAxis(mpu.getAngleY());
     timer = millis();
   }
}
}
