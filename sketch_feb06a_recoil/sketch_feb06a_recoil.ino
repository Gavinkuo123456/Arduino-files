#include <Mouse.h>
int recoil = 13;
void setup() {
  pinMode(recoil,INPUT);
  Mouse.begin();
}

void loop() {
  if(digitalRead(recoil) == HIGH){
    Mouse.press(MOUSE_LEFT);
    Mouse.press(MOUSE_RIGHT);
    Mouse.move(0, 20, 0);
    delay(45);
    Mouse.move(-10,0 , 0);
    for(int i = 0; i<=25 ;i++){
      Mouse.move(1, 1, 0);
      delay(50);
    }
    for(int i = 0; i<=25 ;i++){
      Mouse.move(-1, 1, 0);
      delay(50);
    }
  }
  Mouse.release(MOUSE_LEFT);
  Mouse.release(MOUSE_RIGHT);
}
