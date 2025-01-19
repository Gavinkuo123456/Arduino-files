#include <ESP8266WiFi.h>

int power = 0;
int dir = 2;

// 設定無線基地台SSID跟密碼
const char* ssid     = "gavinroom";     //改成您的SSID
const char* password = "+8860905505182";   //改成您的密碼

// 設定 web server port number 80
WiFiServer server(80);

// 儲存 HTTP request 的變數
String header;

// 儲存目前輸出的狀態
int fanstate = 0;
String output0State = "STOP";

void setup() {

  Serial.begin(115200);
  // 將輸出變數初始化

  pinMode(power, OUTPUT);
  pinMode(dir, OUTPUT);

  // 設定為低電位
  digitalWrite(power, LOW);
  digitalWrite(dir,HIGH);
  delay(5000);
  digitalWrite(dir,LOW);
  delay(5000);
  digitalWrite(power, HIGH);

  // 使用SSID 跟 password 連線基地台
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // 使用COM Port 列出取得的IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
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
              Serial.println("GPIO 0 on");
              fanstate = 0;
              output0State = "Airflow STOP";
              digitalWrite(power,HIGH);
            } else if (header.indexOf("GET /0/1") >= 0) {
              Serial.println("GPIO 0 off");
              fanstate = 2;
              output0State = "Airflow Out";
              digitalWrite(power,LOW);
              digitalWrite(dir,LOW);
            } else if (header.indexOf("GET /0/0") >= 0) {
              Serial.println("GPIO 0 off");
              fanstate = 1;
              output0State = "Airflow In";
              digitalWrite(power,LOW);
              digitalWrite(dir,HIGH);
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
            client.println("<p>Airflow Fan - State " + output0State + "</p>");
            // 按鈕假使狀態是 off, 就要顯示 ON
            if (fanstate == 0) {
              client.println("<p><a href=\"/0/0\"><button class=\"button\">STOP</button></a></p>");
            } else if (fanstate == 1) {
              client.println("<p><a href=\"/0/1\"><button class=\"button\">IN</button></a></p>");
            } else if (fanstate == 2) {
              client.println("<p><a href=\"/0/2\"><button class=\"button\">OUT</button></a></p>");
            }
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
