#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>


const char* ssid = "??????????";
const char* password = "??????????";
 
const int outputPin = D1; 
const int buttonPin = D8;
uint8_t MAC_array[6];
char MAC_char[18]; 
int buttonState = 0;      

WiFiServer server(80);

 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(outputPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(outputPin, 0);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
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


  WiFi.macAddress(MAC_array);
    for (int i = 0; i < sizeof(MAC_array); ++i){
      sprintf(MAC_char,"%s%02x:",MAC_char,MAC_array[i]);
    }
  
    Serial.println(MAC_char);
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

 
void loop() {

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  buttonState = digitalRead(buttonPin);

  int value = LOW;
  if (request.indexOf("/GAR=ACTIVATE") != -1) {
    digitalWrite(outputPin, HIGH);
    value = HIGH;
    delay(500);
    digitalWrite(outputPin, LOW);
    value = LOW;
  } 
 
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Garage is now: ");
 
  if(buttonState == HIGH) {
    client.print("Closed");  
  } else {
    client.print("Open");
  }

  client.println("<br><br>");
  client.println("Click <a href=\"/GAR=ACTIVATE\">here</a> to activate garage door<br>");
    client.println("</html>");
 
  delay(1);
  
  if(buttonState == HIGH) {
    Serial.println("Closed");  
  } else {
    Serial.println("Open");
  }
  Serial.println("Client disconnected");
  Serial.println("");
 
}
