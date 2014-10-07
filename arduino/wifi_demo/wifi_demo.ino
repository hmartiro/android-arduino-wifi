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
static const String PING_MSG = "SOCKET_PING";

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

/**
 * Called whenever a newline-delimited message is received.
 */
void message_received(String msg) {
  Serial.println("[RX] " + msg);
}

void loop() {
  
  // Looks for a connected client
  Client client = server.available();
  
  if (client) {
    
    // Buffer to store received data
    String rx_buffer = "";
    
    // Last time a ping was sent
    int senttime = millis();

    Serial.println("Connected to client.");
    
    while (client.connected()) {
      
      // Read data from the client, and when we hit a newline,
      // invoke the message_received callback and echo the message
      // back to the client.
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
        Serial.println("[TX] " + PING_MSG);
        client.println(PING_MSG);
        senttime = now;
      }
    }
    
    // Give the client time to receive data before closing
    Serial.println("Disconnecting from client.");
    delay(100);
    client.flush();
    client.stop();
  }
}
