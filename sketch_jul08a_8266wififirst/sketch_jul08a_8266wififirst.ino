#include <ESP8266WiFi.h>


const char* ssid = "gavinroom";
const char* password = "+8860905505182";
const char* host = "192.168.0.104";


void setup(void) {
  pinMode(D1, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;
  const int httpport = 8082;

  if (!client.connect(host, httpport)) {
    Serial.println("connect fail");
    return;
  } else {
    Serial.println("connect ok");
  }
  String httpRequest =  String("GET /") + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n" +
                        "\r\n";
  client.print(httpRequest);
  Serial.println("sending");
  Serial.println(httpRequest);
  delay(10);

  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.println(line);
    }
  }

  Serial.println("connect end");
  client.stop();
  delay(5000);
}
