// 動手做12-3：使用 IRremote 程式庫解析紅外線遙控值
// 詳細的程式說明，請參閱第十二章，12-12頁。

#include <IRremote.h>
#include <Servo.h>

Servo servoX, servoY;
const byte RECV_PIN = 3;    // 紅外線接收腳
boolean sw = false;         // 開關狀態，預設為「關」
byte servoPosX = 90;// 伺服器角度，預設為 90 度
byte servoPosY = 90;
const int Motor_E2 = 5; // digital pin 5 of Arduino (PWM)    
const int Motor_E1 = 6;  // digital pin 6 of Arduino (PWM)  
const int Motor_M2 = 10;     // digital pin 7 of Arduino
const int Motor_M1 = 11;


IRrecv irrecv(RECV_PIN);    // 初始化紅外線接收器
decode_results results;      // 儲存紅外線碼解析值

void setup() {
  irrecv.enableIRIn();       // 啟動紅外線接收器
  servoX.attach(8, 500, 2400);   // 連接伺服器
  servoY.attach(9, 500, 2400);
  servoX.write(90); // 設定伺服馬達的旋轉角度
  servoY.write(90);
  pinMode(Motor_M1, OUTPUT);
  pinMode(Motor_M2, OUTPUT);

  
}

void loop() {
  if (irrecv.decode(&results)) { // 如果收到紅外線遙控訊號...
    switch (results.value) { // 讀取解析之後的數值，並且比較...
      case 0x10458: // 若此數值等於「錄影」...
        forward(0, 218);
        break;
      case 0x10459: // 若此數值等於「錄影」...
        backward(0, 218);
        break;
      case 0x1045B: // 若此數值等於「錄影」...
        right(0, 218);
        break;
      case 0x1045A: // 若此數值等於「錄影」...
        left(0, 218);
        break;
      case 0x1045C: // 若此數值等於「錄影」...
        motorstop(0, 0);
        break;
      case 0x458: // 若此數值等於「錄影」...
        forward(0, 218);
        break;
      case 0x459: // 若此數值等於「錄影」...
        backward(0, 218);
        break;
      case 0x45B: // 若此數值等於「錄影」...
        right(0, 218);
        break;
      case 0x45A: // 若此數值等於「錄影」...
        left(0, 218);
        break;
      case 0x45C: // 若此數值等於「錄影」...
        motorstop(0, 0);
        break;
        

      case 0x10406: // 若此數值等於「左方向鍵」...
        if (servoPosX > 60) { // 若伺服器旋轉角度大於 10...
          servoPosX  -= 5;    // 減少旋轉角度 10 度
          servoX.write(servoPosX);
        }
        break;
      case 0x10404: // 若此數值等於「右方向鍵」...
        if (servoPosX < 150) { // 若伺服器旋轉角度小於 170...
          servoPosX += 5;     // 增加旋轉角度 10 度
          servoX.write(servoPosX);
        }
        break;
      case 0x406: // 若此數值等於「左方向鍵」...
        if (servoPosX > 30) { // 若伺服器旋轉角度大於 10...
          servoPosX -= 5;    // 減少旋轉角度 10 度
          servoX.write(servoPosX);
        }
        break;
      case 0x404: // 若此數值等於「右方向鍵」...
        if (servoPosX < 150) { // 若伺服器旋轉角度小於 170...
          servoPosX += 5;     // 增加旋轉角度 10 度
          servoX.write(servoPosX);
        }
        break;


      case 0x10408: // 若此數值等於「左方向鍵」...
        if (servoPosY > 30) { // 若伺服器旋轉角度大於 10...
          servoPosY -= 5;    // 減少旋轉角度 10 度
          servoY.write(servoPosY);
        }
        break;
      case 0x10402: // 若此數值等於「右方向鍵」...
        if (servoPosY < 135) { // 若伺服器旋轉角度小於 170...
          servoPosY += 5;     // 增加旋轉角度 10 度
          servoY.write(servoPosY);
        }
        break;
      case 0x408: // 若此數值等於「左方向鍵」...
        if (servoPosY > 60) { // 若伺服器旋轉角度大於 10...
          servoPosY -= 5;    // 減少旋轉角度 10 度
          servoY.write(servoPosY);
        }
        break;
      case 0x402: // 若此數值等於「右方向鍵」...
        if (servoPosY < 135) { // 若伺服器旋轉角度小於 170...
          servoPosY += 5;     // 增加旋轉角度 10 度
          servoY.write(servoPosY);
        }
        break;
    }
    irrecv.resume(); // 準備接收下一筆資料
  }
}


void motorstop(byte flag, byte motorspeed)
{
  Serial.println("Stop!");
  digitalWrite( Motor_E1, motorspeed);
  digitalWrite( Motor_E2, motorspeed);

}

void forward(byte flag, byte motorspeed)
{
  Serial.println("Forward!");

  digitalWrite( Motor_M1, HIGH);
  digitalWrite( Motor_M2, HIGH);

  analogWrite( Motor_E1, motorspeed);
  analogWrite( Motor_E2, motorspeed);
  
}

void backward(byte flag, byte motorspeed)
{
  digitalWrite( Motor_M1, LOW);
  digitalWrite( Motor_M2, LOW);
  
  analogWrite( Motor_E1, motorspeed);
  analogWrite( Motor_E2, motorspeed);

}

void right(byte flag, byte motorspeed)
{
  Serial.println("Turn Right! ");

  digitalWrite( Motor_M1, HIGH);
  digitalWrite( Motor_M2, HIGH);

  analogWrite( Motor_E1, motorspeed);
  analogWrite( Motor_E2, 0);
  
}

void left(byte flag, byte motorspeed)
{
  Serial.println("Turn Left!");

  digitalWrite( Motor_M1, HIGH);
  digitalWrite( Motor_M2, HIGH);

  analogWrite( Motor_E1, 0);
  analogWrite( Motor_E2, motorspeed);
  
}
