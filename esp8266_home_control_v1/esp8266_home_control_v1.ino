#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP_Mail_Client.h>
#include <ArduinoJson.h>
#include <RtcDS1302.h>
#include <OneWire.h>
#include <DallasTemperature.h>

ThreeWire myWire(D6, D5, D7); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
int power = 0;
int dir = 2;

const char *ssid     = "gavinroom";
const char *password = "+8860905505182";

void get_Weather();
void sentmail();
void webcontrol();
void autofan();

// =====  setup =====
const char *host = "192.168.0.104";
const int httpPort = 8084;

const char* elementName;
String tempout;
String tempin;
float tempread;
float tempdatacenter;

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "ESP622104@gmail.com"
#define AUTHOR_PASSWORD "vdjcsxqcjfdoftct"
#define SENDER_NAME "Esp8266"
#define MAIL_SUBJECT "天氣觀測結果"

#define RECIPIENT "Gavin"
#define RECIPIENT_EMAIL "gavinkuo123456@gmail.com"

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

SMTPSession smtp;

void smtpCallback(SMTP_Status status);


// 設定 web server port number 80
WiFiServer server(80);

// 儲存 HTTP request 的變數
String header;

// 儲存目前輸出的狀態
int fanstate = 0;
String output0State = "STOP";

void setup() {
  Serial.begin(115200);
  Rtc.Begin();
  delay(10);

  pinMode(power, OUTPUT);
  pinMode(dir, OUTPUT);

  // 設定為低電位
  digitalWrite(power, LOW);
  digitalWrite(dir, LOW);
  delay(5000);
  digitalWrite(dir, HIGH);
  delay(5000);
  digitalWrite(power, HIGH);


  // ===== We start by connecting to a WiFi network =====
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // ===== Wait for the connection, flashing the LED while we wait =====
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  // ===== Connect successful =====
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  sensors.begin();

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
  get_Weather();
  Serial.println(elementName);
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();

  if (now.Minute() % 5 == 00 && now.Second() == 00) {
    get_Weather();
    delay(1000);
  }

  if (now.Minute() == 00 && now.Second() == 00) {
    get_Weather();
    sendmail();
    delay(1000);
  }
  if (now.Minute() % 2 == 00 && now.Second() == 00) {
    sensors.requestTemperatures();
    tempread = sensors.getTempCByIndex(0);
    Serial.print(tempread);
    Serial.println("ºC");
    tempin = String(tempread);
    delay(1000);
  }
  webcontrol();

  if (fanstate == 0) {
    digitalWrite(power, HIGH);
    digitalWrite(dir, HIGH);
  } else if (fanstate == 1) {
    digitalWrite(power, LOW);
    digitalWrite(dir, HIGH);
  } else if (fanstate == 2) {
    digitalWrite(power, LOW);
    digitalWrite(dir, LOW);
  } else if (fanstate == 3) {
    autofan();
  }
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

void sendmail() {
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
  String htmlMsg = head + temp + last;
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
void get_Weather() {
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
  tempout = String(elementName);
  tempdatacenter = tempout.toFloat();
  client.stop();

  sensors.requestTemperatures();
  tempread = sensors.getTempCByIndex(0);
  Serial.print(tempread);
  Serial.println("ºC");
  tempin = String(tempread);
}
void webcontrol() {
  WiFiClient client = server.available();   // 等待 clients 連線

  if (client) {                             // 假使新的用戶端連線

    Serial.println("New Client.");          // 從序列 Port印出訊息內容
    String currentLine = "";                // 清空這行的內容
    while (client.connected()) {            // 當 client繼續連線持續執行迴圈
      if (client.available()) {             // 假使從 client 有讀到字元
        char c = client.read();             // 讀取這個字元
        Serial.write(c);                    // 印出這個字元在串列視窗
        header += c;
        if (c == '\n') {                    // 假使是換行符號

          // 假使目前的一行是空白且有兩個新行，就結束 client HTTP 的要求
          if (currentLine.length() == 0) {
            // HTTP 表頭開始時，會有回應碼 response code (如： HTTP/1.1 200 OK)
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // 將 GPIOs 開或關
            if (header.indexOf("GET /0/2") >= 0)
            {
              fanstate = 3;
              output0State = "Auto Mode";
            } else if (header.indexOf("GET /0/1") >= 0) {
              fanstate = 2;
              output0State = "Airflow Out";
            } else if (header.indexOf("GET /0/0") >= 0) {
              fanstate = 1;
              output0State = "Airflow In";
            } else if (header.indexOf("GET /0/3") >= 0) {
              fanstate = 0;
              output0State = "Airflow STOP";
            }
            // 顯示 HTML 網頁
            client.println("<html>");
            client.println("<head>");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // 設定 on/off 按鈕的CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // 網頁表頭
            client.println("<body><h1>GavinRoom control panel</html>");

            // 顯示現在GPIO 0 按鈕的狀態是 ON/OFF
            client.println("<p>Fan : " + output0State + "</p>");
            // 按鈕假使狀態是 off, 就要顯示 ON
            if (fanstate == 0) {
              client.println("<p><a href=\"/0/0\"><button class=\"button\">STOP</button></a></p>");
            } else if (fanstate == 1) {
              client.println("<p><a href=\"/0/1\"><button class=\"button\">IN</button></a></p>");
            } else if (fanstate == 2) {
              client.println("<p><a href=\"/0/2\"><button class=\"button\">OUT</button></a></p>");
            } else if (fanstate == 3) {
              client.println("<p><a href=\"/0/3\"><button class=\"button\">AUTO</button></a></p>");
            }
            client.println("<p>Temperature outside : " + tempout + "C</p>");
            client.println("<p>Temperature inside : " + tempin + "C</p>");
            client.println("</body></html>");

            // 使用空白行結束 HTTP回應
            client.println();
            break;
          } else {   // 假使有新的一行, 清除目前這一行
            currentLine = "";
          }
        } else if (c != '\r') {  // 讀取到的不是換行符號
          currentLine += c;      // 增加一個字元在本行最後
        }
      }
    }

    // 清除表頭變數
    header = "";
    // 關閉連線 connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void autofan() {
  if (tempread > tempdatacenter) {
    digitalWrite(power, LOW);
    digitalWrite(dir, LOW);
  } else {
    digitalWrite(power, HIGH);
    digitalWrite(dir, HIGH);
  }
}
