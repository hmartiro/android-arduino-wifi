/**
 * WiFi communication demo using the WiFly module.
 * 
 * Author: Hayk Martirosyan
 */
 
#include "WiFly.h"
#include "Credentials.h"

// Server object, created with the desired port
Server server(80);

// How often to ping (milliseconds)
static const int PING_TIME = 1000;

// Last time a ping was sent
int senttime = millis();

void setup() {
  
  Serial.begin(115200);
  
  Serial.println("Initiating WiFly...");
  WiFly.begin();
  
  Serial.println("Joining network " + String(ssid) + "...");
  if (!WiFly.join(ssid, passphrase, true)) {
    
    Serial.println("ERROR: Failed to connect!");
    while (1) {}
  }
  
  String ip = String(WiFly.ip());
  Serial.println("IP: " + ip);
  
  server.begin();
}

void message_received(String msg) {
  Serial.println("[RX] " + msg);
}

void loop() {
  
  Client client = server.available();
  
  if (client) {
    String rx_buffer = "";
    Serial.println("Connected to client.");
    
    while (client.connected()) {
      
      if(client.available()) {
        while(client.available()) {
          
          char c = client.read();
          
          if(c == '\n') {
            
            message_received(rx_buffer);
            
            client.println(rx_buffer);
            Serial.println("[TX] " + rx_buffer);
            
            rx_buffer = "";
            
          } else {
            rx_buffer += c;
          }
        }
      }
      
      // Send a ping message once a second to keep the connection open
      int now = millis();
      if (now - senttime > PING_TIME) {
        String msg = "SOCKET_PING";
        Serial.println("[TX] " + msg);
        client.println(msg);
        senttime = now;
      }
    }
    
    // Give the client time to receive data before closing
    Serial.println("Disconnecting from client.");
    delay(100);
    client.stop();
  }
}
