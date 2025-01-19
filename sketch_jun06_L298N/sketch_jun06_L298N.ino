#include <IRremote.h>

const int In1 = 4;
const int In2 = 5;
const int In3 = 6;
const int In4 = 7;
const int In5 = 8;
const int In6 = 9;
const int In7 = 10;
const int In8 = 11;
const byte RECV_PIN = 3;

IRrecv irrecv(RECV_PIN);
decode_results results;


void setup() {
  irrecv.enableIRIn();
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(In5, OUTPUT);
  pinMode(In6, OUTPUT);
  pinMode(In7, OUTPUT);
  pinMode(In8, OUTPUT);
}
void loop() {
  if (irrecv.decode(&results)) { // 如果收到紅外線遙控訊號...
    switch (results.value) { // 讀取解析之後的數值，並且比較...
      case 0x10458: // 若此數值等於「錄影」...
        mfront();
        break;
      case 0x10459: // 若此數值等於「錄影」...
        mback();
        break;
      case 0x1045B: // 若此數值等於「錄影」...
        mleft();
        break;
      case 0x1045A: // 若此數值等於「錄影」...
        mright();
        break;
      case 0x1045C: // 若此數值等於「錄影」...
        mstop();
        break;
      case 0x458: // 若此數值等於「錄影」...
        mfront();
        break;
      case 0x459: // 若此數值等於「錄影」...
        mback();
        break;
      case 0x45B: // 若此數值等於「錄影」...
        mleft();
        break;
      case 0x45A: // 若此數值等於「錄影」...
        mright();
        break;
      case 0x45C: // 若此數值等於「錄影」...
        mstop();
        break;
        

      case 0x10406: // 若此數值等於「左方向鍵」...
        aleft();
        break;
      case 0x10404: // 若此數值等於「右方向鍵」...
        aright();
        break;
      case 0x406: // 若此數值等於「左方向鍵」...
        aleft();
        break;
      case 0x404: // 若此數值等於「右方向鍵」...
        aright();
        break;


      case 0x10408: // 若此數值等於「左方向鍵」...
        aback();
        break;
      case 0x10402: // 若此數值等於「右方向鍵」...
        afront();
        break;
      case 0x408: // 若此數值等於「左方向鍵」...
        aback();
        break;
      case 0x402: // 若此數值等於「右方向鍵」...
        afront();
        break;

      case 0x10405: // 若此數值等於「左方向鍵」...
        astop();
        break;
      case 0x405: // 若此數值等於「右方向鍵」...
        astop();
        break;
    }
    irrecv.resume(); // 準備接收下一筆資料
  }
}


void mstop() {
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}
void mfront() {
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}
void mback() {
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}
void mleft() {
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}
void mright() {
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}

void astop() {
  digitalWrite(In5, LOW);
  digitalWrite(In6, LOW);
  digitalWrite(In7, LOW);
  digitalWrite(In8, LOW);
}
void afront() {
  digitalWrite(In5, HIGH);
  digitalWrite(In6, LOW);
  digitalWrite(In7, HIGH);
  digitalWrite(In8, LOW);
}
void aback() {
  digitalWrite(In5, LOW);
  digitalWrite(In6, HIGH);
  digitalWrite(In7, LOW);
  digitalWrite(In8, HIGH);
}
void aleft() {
  digitalWrite(In5, LOW);
  digitalWrite(In6, LOW);
  digitalWrite(In7, HIGH);
  digitalWrite(In8, LOW);
}
void aright() {
  digitalWrite(In5, HIGH);
  digitalWrite(In6, LOW);
  digitalWrite(In7, LOW);
  digitalWrite(In8, LOW);
}
