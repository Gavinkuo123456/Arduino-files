#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
int ledR = 9;
int ledG = 10;
int buzzer = 11;
const byte ROWS = 4;  //four rows
const byte COLS = 3;  //three columns
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 7, 8, 6, 5 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 3, 2 };     //connect to the column pinouts of the keypad

String passcode = "7355608";
String inputCode = "";

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  // put your setup code here, to run once:
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("*******");
  tone(buzzer, 1500, 100);
  delay(110);
  tone(buzzer, 1500, 100);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      lcd.setCursor(4, 0);
      lcd.print("*******");
      inputCode = "";
    } else if (inputCode.length() >= 7) {
      lcd.setCursor(4, 0);
      lcd.print("*******");
      inputCode = "";
    } else {
      lcd.setCursor(4 + inputCode.length(), 0);
      lcd.print(key);
      inputCode += key;
    }
  }
  if (inputCode.length() >= 7 && inputCode != "7355608") {
    lcd.setCursor(4, 0);
    lcd.print("*******");
    inputCode = "";
  }

  if (inputCode == "7355608") {
    inputCode = "";
    lcd.setCursor(2, 0);
    lcd.print("Bomb planted");
    for (int i = 1000; i >= 50; i = i - 10) {
      digitalWrite(ledR, HIGH);
      tone(buzzer, 2500, 100);
      delay(i / 2);
      digitalWrite(ledR, LOW);
      delay(i / 2);
    }
    for (int i = 0; i <= 8; i++) {
      digitalWrite(ledG, HIGH);
      tone(buzzer, 2800, 50);
      delay(100);
    }
    digitalWrite(ledG, LOW);

    lcd.setCursor(1, 0);
    lcd.print("Terrorist win");
    delay(3000);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("*******");
  }
}
