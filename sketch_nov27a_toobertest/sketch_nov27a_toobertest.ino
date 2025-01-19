void setup() {
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  pinMode(11,INPUT);

  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

}

void loop() {
  if(digitalRead(8) == 1){
    digitalWrite(4,HIGH);
    tone(3,440,5);
  }
  if(digitalRead(9) == 1){
    digitalWrite(5,HIGH);
    tone(3,500,5);
  }
  if(digitalRead(10) == 1){
    digitalWrite(6,HIGH);
    tone(3,600,5);
  }
  if(digitalRead(11) == 1){
    digitalWrite(7,HIGH);
    tone(3,700,5);
  }
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
}
