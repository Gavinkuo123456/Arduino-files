#include <ESP8266WiFi.h>
#include <aREST.h>

aREST rest = aREST();


// WiFi parameters
const char* ssid = "gavinroom";
const char* password = "+8860905505182";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT 80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
int x;
int y;


void setup(void)
{  
  // Start Serial
  Serial.begin(115200);

  
  // Init variables and expose them to REST API
  rest.variable("x",&x);
  rest.variable("y",&y);
    
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");
  
  // Connect to WiFi
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
  Serial.println(WiFi.localIP());
  
}

void loop() {
  x = random(0, 100);
  y = random(0, 100);
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}
