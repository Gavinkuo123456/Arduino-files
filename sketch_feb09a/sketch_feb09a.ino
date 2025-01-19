#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <TM1637Display.h>


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// 設定 TM1637 接腳
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);
boolean colon = true ;
int t = 0;
  
void setup()
{
  pinMode(3,INPUT);
  display.setBrightness(0x0f);
  uint8_t data[] = {0x0,0x0,0x0,0x0};
  display.setSegments(data);
  mlx.begin(); 
}

void loop()
{
  t = mlx.readObjectTempC()*10;
  display.showNumberDecEx(t,(0x80 >> 1),true ,3 ,0);
  delay(100);
  if(digitalRead(4) == HIGH){
    delay(3000);
  }
}
