#include <Keypad.h>
#include <Wire.h>

#define KEY_ROWS 4
#define KEY_COLS 3  // 薄膜按鍵的行
// 設置按鍵模組
char keymap[KEY_ROWS][KEY_COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

int lockpin = 11;
int doorsenser = 2;
int opendoor = 12;

bool unlock = false;
bool door = false;


byte rowPins[KEY_ROWS] = {10, 9, 8, 7};
byte colPins[KEY_COLS] = {6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);

String passcode = "10648";   
String inputCode = "";      // 暫存用戶的按鍵字串
bool acceptKey = true;      // 代表是否接受用戶按鍵輸入的變數，預設為「接受」

void resetLocker() {
  acceptKey = true;
  inputCode = "";
  unlock = false;
  digitalWrite(lockpin,LOW);
}

// 比對用戶輸入的密碼
void checkPinCode() {
  acceptKey = false;  // 暫時不接受用戶按鍵輸入

  if (inputCode == passcode) {
    unlock = true;
  } else {
    unlock = false;
    resetLocker();
  }
}

void setup() {
  resetLocker();
  pinMode(lockpin,OUTPUT);
  pinMode(doorsenser,INPUT);
  pinMode(opendoor,INPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  char key = keypad.getKey();
  door = digitalRead(doorsenser);

  // 若目前接受用戶輸入，而且有新的字元輸入…
  if (acceptKey == true && key != NO_KEY) {
    if (key == '*') {   // 清除畫面
      inputCode = "";
    } else if (key == '#') {  // 比對輸入密碼
      checkPinCode();
    } else {
      inputCode += key;  // 儲存用戶的按鍵字元
    }
  }
  if(acceptKey == false&&unlock == true){
    digitalWrite(lockpin,HIGH);
    while(door == true){
      digitalWrite(13,HIGH);
      delay(50);
      digitalWrite(13,LOW);
      delay(50);
      door = digitalRead(doorsenser);
    }
    while(door == false){
      digitalWrite(13,HIGH);
      door = digitalRead(doorsenser);
    }
    while(door == true){
      digitalWrite(13,LOW);
      delay(1000);
      resetLocker();
      break;
    }
  }
  if(digitalRead(opendoor) == true){
    acceptKey = false;
    unlock = true;
  }
  if(door == false && unlock == false){
    while(door == false){
      door = digitalRead(doorsenser);
      digitalWrite(lockpin,HIGH);
    }
    resetLocker();
  }
}
