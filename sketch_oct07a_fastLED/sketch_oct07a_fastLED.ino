#include "FastLED.h"            // 此示例程序需要使用FastLED库
#include <IRremote.hpp>

#define NUM_LEDS 77             // LED灯珠数量
#define LED_DT 5                // Arduino输出控制信号引脚
#define LED_TYPE WS2812         // LED灯带型号
#define COLOR_ORDER GRB         // RGB灯珠中红色、绿色、蓝色LED的排列顺序

uint8_t max_bright = 255;       // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高

CRGB leds[NUM_LEDS];            // 建立光带leds

uint8_t beginHue = 0;
uint8_t deltaHue = 1;

int mode = 0;

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带

  FastLED.setBrightness(max_bright);                            // 设置光带亮度

  IrReceiver.begin(3, DISABLE_LED_FEEDBACK); // PIN3收IR
  Serial.begin(115200);
}

uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void loop () {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    if (IrReceiver.decodedIRData.decodedRawData == 0x10400 || IrReceiver.decodedIRData.decodedRawData == 0x400) { //如果按下遙控器的特定鍵，就顯示訊息！
      mode = 0;
    }
    if (IrReceiver.decodedIRData.decodedRawData == 0x10401 || IrReceiver.decodedIRData.decodedRawData == 0x401) { //如果按下遙控器的特定鍵，就顯示訊息！
      mode = 1;
    }
    if (IrReceiver.decodedIRData.decodedRawData == 0x10402 || IrReceiver.decodedIRData.decodedRawData == 0x402) { //如果按下遙控器的特定鍵，就顯示訊息！
      mode = 2;
    }
    if (IrReceiver.decodedIRData.decodedRawData == 0x10403 || IrReceiver.decodedIRData.decodedRawData == 0x403) { //如果按下遙控器的特定鍵，就顯示訊息！
      mode = 3;
    }
    if (IrReceiver.decodedIRData.decodedRawData == 0x10404 || IrReceiver.decodedIRData.decodedRawData == 0x404) { //如果按下遙控器的特定鍵，就顯示訊息！
      mode = 4;
    }
    if (IrReceiver.decodedIRData.decodedRawData == 0x10405 || IrReceiver.decodedIRData.decodedRawData == 0x405) { //如果按下遙控器的特定鍵，就顯示訊息！
      mode = 5;
    }
    /*if (IrReceiver.decodedIRData.decodedRawData == 0x10406 || IrReceiver.decodedIRData.decodedRawData == 0x406) { //如果按下遙控器的特定鍵，就顯示訊息！
      mode = 6;
    }*/
    IrReceiver.resume(); // Enable receiving of the next value
  }
  if (mode == 0) {
    off();
  }
  if (mode == 1) {
    rainbow();
  }
  if (mode == 2) {
    confetti();
  }
  if (mode == 3) {
    juggle();
  }
  if (mode == 4) {
    white();
  }
  if (mode == 5) {
    DJ();
  }
  if (mode == 6) {
    sinelon();
  }
  delay(40);

}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  FastLED.show();

}

void off() {
  fadeToBlackBy( leds, NUM_LEDS, 20);
  FastLED.show();
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  FastLED.show();
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
}
void white() {
  fill_solid(leds, 77, CHSV (0 , 0 , 128));
  FastLED.show();
}

void DJ() {
  fill_solid(leds, 77, CHSV(random(0, 360), 255, 255));
  FastLED.show();
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 5);
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( gHue, 255, 192);
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  FastLED.show();

}
