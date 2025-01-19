#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP_Mail_Client.h>
#include <ArduinoJson.h>

void get_Weather(); // Function 原型

// ===== Wifi setup =====
const char *ssid     = "gavinroom";
const char *password = "+8860905505182";

// =====  setup =====
const char *host = "192.168.0.104";
const int httpPort = 8084;

// ===== Hardware setup =====
const int buttonPin = D8;     // the number of the pushbutton pin
const int ledPin = D1;        // the number of the LED pin

// ===== Variables will change =====
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
const char* elementName;

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "gavinkuo123456@gmail.com"
#define AUTHOR_PASSWORD "jdvpujrwpqkexxyf"
#define SENDER_NAME "Esp8266"
#define MAIL_SUBJECT "天氣觀測結果"

#define RECIPIENT "Gavin"
#define RECIPIENT_EMAIL "gavinkuo123456@gmail.com"

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  delay(10);

  // ===== We start by connecting to a WiFi network =====
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // ===== Wait for the connection, flashing the LED while we wait =====
  int led = HIGH;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    digitalWrite(ledPin, led);
    led = !led;
    Serial.print(".");
  }
  digitalWrite(ledPin, LOW);

  // ===== Connect successful =====
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  smtp.debug(1);

  /* 設定取得寄信狀態的回呼函式 */
  smtp.callback(smtpCallback);

  /* 宣告SMTP郵件伺服器連線物件 */
  ESP_Mail_Session session;

  /* 設定SMTP郵件伺服器連線物件 */
  session.server.host_name = SMTP_HOST;      // 設定寄信的郵件伺服器名稱
  session.server.port = SMTP_PORT;           // 郵件伺服器的埠號
  session.login.email = AUTHOR_EMAIL;        // 你的帳號
  session.login.password = AUTHOR_PASSWORD;  // 密碼
  /* 設置時區 */
  session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  session.time.gmt_offset = 8;        // 台北時區
  session.time.day_light_offset = 0;  // 無日光節約時間

  /* 宣告SMTP訊息物件 */
  SMTP_Message message;

  /* 設定郵件標頭 */
  message.sender.name = SENDER_NAME;    // 寄信人的名字
  message.sender.email = AUTHOR_EMAIL;  // 寄信人的e-mail
  message.subject = MAIL_SUBJECT;       // 信件主旨
  message.addRecipient(RECIPIENT, RECIPIENT_EMAIL);  // "收信人的名字", "收信人的e-mail"

  /* 設定郵件內容（HTML格式訊息） */
  String htmlMsg = "<div style=\"color:#000000;\"><h1>成功開機訊息測試</h1><p>- 從ESP32開發板傳送</p></div>";
  message.html.content = htmlMsg.c_str();  // 設定信件內容
  message.text.charSet = "utf-8";

  while (!smtp.connect(&session)) {
    Serial.print("*");
    delay(5);
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("寄信時出錯了：" + smtp.errorReason());
  }

}

void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("*****************************");
  Serial.print("Connecting to ");
  Serial.println(host);
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  String url = "/api/v1/rest/datastore/O-A0003-001?Authorization=";
  String Auth = "CWB-0305539E-FEE4-45F1-829A-5239DA24147D";
  String loca = "&limit=1&offset=11";
  Serial.println("Requesting URL");

  // This will send the request to the server
  client.print(String("GET ") + url + Auth + loca + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.


  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    client.stop();
    return;
  }
  /*
      while (client.available() && client.peek() != '{')
    {
      char c = 0;
      client.readBytes(&c, 1);
      Serial.print(c);
      Serial.println("BAD");
    }
  */
  /*
    while (client.available() && client.peek() != '{')
    {
    char c = 0;
    client.readBytes(&c, 1);
    Serial.print(c);
    Serial.println("BAD");
    }
  */

  while (client.connected()) {
    if (client.available() && client.peek() != '{')
    {
      char c = 0;
      client.readBytes(&c, 1);
      Serial.print(c);
      Serial.println("BAD");
    } else if (client.available() && client.peek() == '{') {

      DynamicJsonDocument doc(3072);

      DeserializationError error = deserializeJson(doc, client);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      Serial.print("start Parsering, status : ");
      Serial.println(error.f_str());

      const char* success = doc["success"]; // "true"

      const char* result_resource_id = doc["result"]["resource_id"]; // "O-A0003-001"

      for (JsonObject result_field : doc["result"]["fields"].as<JsonArray>()) {

        const char* result_field_id = result_field["id"]; // "lat", "lon", "locationName", "stationId", ...
        const char* result_field_type = result_field["type"]; // "Double", "Double", "String", "String", ...

      }

      JsonObject records_location_0 = doc["records"]["location"][0];
      const char* records_location_0_lat = records_location_0["lat"]; // "25.031331"
      const char* records_location_0_lon = records_location_0["lon"]; // "121.528056"
      const char* records_location_0_locationName = records_location_0["locationName"]; // "大安森林"
      const char* records_location_0_stationId = records_location_0["stationId"]; // "CAAH60"

      const char* records_location_0_time_obsTime = records_location_0["time"]["obsTime"]; // "2023-07-25 ...

      for (JsonObject records_location_0_weatherElement_item : records_location_0["weatherElement"].as<JsonArray>()) {

        const char* records_location_0_weatherElement_item_elementName = records_location_0_weatherElement_item["elementName"];
        const char* records_location_0_weatherElement_item_elementValue = records_location_0_weatherElement_item["elementValue"];

      }

      for (JsonObject records_location_0_parameter_item : records_location_0["parameter"].as<JsonArray>()) {

        const char* records_location_0_parameter_item_parameterName = records_location_0_parameter_item["parameterName"];
        const char* records_location_0_parameter_item_parameterValue = records_location_0_parameter_item["parameterValue"];
      }
      elementName = doc["records"]["location"][0]["weatherElement"][3]["elementValue"];
    } else {
      Serial.println("no data");
    }
  }

  Serial.println();
  Serial.println("closing connection");
  Serial.println("=========================================");
  client.stop();
  digitalWrite(ledPin, LOW);

  Serial.println(elementName);

  smtp.debug(1);

  /* 設定取得寄信狀態的回呼函式 */
  smtp.callback(smtpCallback);

  /* 宣告SMTP郵件伺服器連線物件 */
  ESP_Mail_Session session;

  /* 設定SMTP郵件伺服器連線物件 */
  session.server.host_name = SMTP_HOST;      // 設定寄信的郵件伺服器名稱
  session.server.port = SMTP_PORT;           // 郵件伺服器的埠號
  session.login.email = AUTHOR_EMAIL;        // 你的帳號
  session.login.password = AUTHOR_PASSWORD;  // 密碼
  /* 設置時區 */
  session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  session.time.gmt_offset = 8;        // 台北時區
  session.time.day_light_offset = 0;  // 無日光節約時間

  /* 宣告SMTP訊息物件 */
  SMTP_Message message;

  /* 設定郵件標頭 */
  message.sender.name = SENDER_NAME;    // 寄信人的名字
  message.sender.email = AUTHOR_EMAIL;  // 寄信人的e-mail
  message.subject = MAIL_SUBJECT;       // 信件主旨
  message.addRecipient(RECIPIENT, RECIPIENT_EMAIL);  // "收信人的名字", "收信人的e-mail"

  /* 設定郵件內容（HTML格式訊息） */
  String temp = String(elementName);
  String head = "<div style=\"color:#000000;\"><h1>目前天氣觀測結果</h1><p>攝氏";
  String last = "度- 從ESP32開發板傳送</p></div>";
  String htmlMsg = head + temp+ last;
  message.html.content = htmlMsg.c_str();  // 設定信件內容
  message.text.charSet = "utf-8";

  while (!smtp.connect(&session)) {
    Serial.print("*");
    delay(5);
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("寄信時出錯了：" + smtp.errorReason());
  }
  delay(60000);
}

void smtpCallback(SMTP_Status status) {
  /* 顯示目前的狀態 */
  Serial.println(status.info());

  /* 顯示傳送結果 */
  if (status.success()) {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("訊息傳送成功：%d\n", status.completedCount());
    ESP_MAIL_PRINTF("訊息傳送失敗：%d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (int i = 0; i < smtp.sendingResult.size(); i++) {
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;

      ESP_MAIL_PRINTF("訊息編號：%d\n", i + 1);
      ESP_MAIL_PRINTF("狀態：%s\n", result.completed ? "成功" : "失敗");
      ESP_MAIL_PRINTF("日期/時間：%s\n", asctime(localtime(&ts)));
      ESP_MAIL_PRINTF("收信人：%s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("主旨：%s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");
    smtp.sendingResult.clear();
  }
}
