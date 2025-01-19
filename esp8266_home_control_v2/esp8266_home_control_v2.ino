#include <ESP8266WiFi.h>
#include <RtcDS1302.h>
 
const char* ssid = "gavinroom";
const char* password = "+8860905505182";
 
int ledPin = LED_BUILTIN;
WiFiServer server(80);

int power = 0;
int dir = 2;

ThreeWire myWire(12,14,13); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  pinMode(power,OUTPUT);
  pinMode(dir,OUTPUT);

  digitalWrite(power, HIGH);
  digitalWrite(dir,HIGH);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  Rtc.Begin();
 
}
 
void loop() {
  RtcDateTime now = Rtc.GetDateTime();
  const RtcDateTime& dt = now;

  char datestring[26];

  snprintf_P(datestring, 
    countof(datestring),
    PSTR("%04u/%02u/%02u %02u:%02u:%02u"),
    dt.Year(),
    dt.Month(),
    dt.Day(),
    dt.Hour(),
    dt.Minute(),
    dt.Second() );

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  int timeout = 0;
  while(!client.available() && timeout <=10000){
    timeout++;
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = HIGH;
  if (request.indexOf("/STOP") != -1)  {
    digitalWrite(power, HIGH);
    digitalWrite(dir,HIGH);
    value = LOW;
  }
  if (request.indexOf("/IN") != -1)  {
    digitalWrite(power, LOW);
    digitalWrite(dir,LOW);
    value = HIGH;
  }
  if (request.indexOf("/OUT") != -1)  {
    digitalWrite(power, LOW);
    digitalWrite(dir,HIGH);
    value = HIGH;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  client.println("<head>");
  client.println("<link rel=\"icon\" href=\"data:,\">");

            // 設定 on/off 按鈕的CSS
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(".button2 {background-color: #77878A;}</style></head>");

// 網頁表頭
  client.println("<body><h1>GavinRoom Fan control panel</html>");

  if (request.indexOf("/STOP") != -1)  {
    client.println("<h2>Fan state: STOP");
  }
  if (request.indexOf("/IN") != -1)  {
    client.println("<h2>Fan state: Airflow IN");
  }
  if (request.indexOf("/OUT") != -1)  {
    client.println("<h2>Fan state: Airflow OUT");
  }

  client.println("<br><br>");
  client.println("<a href=\"/STOP\"\"><button class=\"button\">STOP</button></a><br>");
  client.println("<a href=\"/IN\"\"><button class=\"button\">IN</button></a><br>");
  client.println("<a href=\"/OUT\"\"><button class=\"button\">OUT</button></a><br>");
  client.println("</body></html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
