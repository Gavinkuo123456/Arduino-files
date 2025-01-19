#include <ESP8266WiFi.h>

const char* ssid = "gavinroom";
const char* password = "+8860905505182";

const char* host = "dweet.io";

int x;
int y;
int prey;
int z;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());
  y = 0;
  prey = -1;
  z = 0;
}

void loop() {
  x = random(0, 100);
  
  Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // This will send the request to the server
  client.print(String("GET /dweet/for/gavinhome?x=") + String(x) + "&y=" + String(y) + "&z=" + String(z) +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.connected()){
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
  Serial.println();
  Serial.println("closing connection");
  Serial.println();
  
  // Repeat every 10 seconds
  delay(500);
  if(prey<y){
    prey = y;
    y = y+5;
    if(y == 100){
      prey = y;
      y = 95;
    }
  }else if(prey>y){
    prey = y;
    y = y-5;
    if(y == 0){
      prey = y;
      y = 5;
    }
  }
  z = z+5;
  if(z == 100){
    z = 0;
  }
}
