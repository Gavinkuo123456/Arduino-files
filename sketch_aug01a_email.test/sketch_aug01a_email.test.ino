#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "gavinroom"
#define WIFI_PASSWORD "+8860905505182"
// 採用Gmail的郵件伺服器
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "gavinkuo123456@gmail.com"
#define AUTHOR_PASSWORD "jdvpujrwpqkexxyf"
#define SENDER_NAME "Esp8266"
#define MAIL_SUBJECT "testing" 

#define RECIPIENT "Gavin"
#define RECIPIENT_EMAIL "gavinkuo04@gmail.com"

/* 宣告用於寄信的SMTP Session物件 */
SMTPSession smtp;

/* 取得寄信狀態的回呼函式 */
void smtpCallback(SMTP_Status status);

void setup(){
  Serial.begin(115200);
  Serial.print("\n連接Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWi-Fi已連線。");
  Serial.println("IP位址：");
  Serial.println(WiFi.localIP());
  Serial.println();

  /** 是否在序列埠監控視窗顯示除錯訊息
   * 0代表不要
   * 1代表顯示簡單除錯訊息
  */
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
  String htmlMsg = "<div style=\"color:#2f4468;\"><h1>學程式就是要動手實作</h1><p>- 從ESP32開發板傳送</p></div>";
  message.html.content = htmlMsg.c_str();  // 設定信件內容
  message.text.charSet = "utf-8";          // 設定訊息文字的編碼

  /* 連線到郵件伺服器 */
  if (!smtp.connect(&session))
    return;

  /* 開始寄信 */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("寄信時出錯了：" + smtp.errorReason());
}

void loop(){

}

/* 取得信件寄送狀態的回呼函式本體 */
void smtpCallback(SMTP_Status status){
  /* 顯示目前的狀態 */
  Serial.println(status.info());

  /* 顯示傳送結果 */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("訊息傳送成功：%d\n", status.completedCount());
    ESP_MAIL_PRINTF("訊息傳送失敗：%d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (int i = 0; i < smtp.sendingResult.size(); i++){
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
