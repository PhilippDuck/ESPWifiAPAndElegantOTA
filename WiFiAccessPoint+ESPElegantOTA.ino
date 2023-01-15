/* JSON example of user list
  {
    "users": [
      {
        "name": "Philipp",
        "uuid": "abb10ea6-c366-40ef-88c4-8bd11cee77b6",
        "level": 9,
        "expiryDate": "00-00-0000"
      },
      {
        "name": "Patrick",
        "uuid": "abb10ea6-c366-40ef-88c4-8bd11cee77b6",
        "level": 9,
        "expiryDate": "00-00-0000"
      }
    ]
  }
  
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <ESPAsyncTCP.h>

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;
const char* PARAM_MESSAGE = "message";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

  if(type == WS_EVT_CONNECT){

    Serial.println("Websocket client connection received");
    client->text("Hello from ESP32 Server");

  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");

  }
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response and Philipp is cool!");
  });
  AsyncElegantOTA.begin(&server, "admin", "password");    // Start AsyncElegantOTA

  server.on("/test", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {

}
