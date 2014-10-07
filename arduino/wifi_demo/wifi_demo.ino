/**
 * WiFi communication demo using the WiFly module.
 *
 * Author: Hayk Martirosyan
 */
 
#include "WiFly.h"
#include "Credentials.h"

// How often to ping (milliseconds)
static const int PING_TIME = 1000;

// Special messages
static const String PING_MSG = "SOCKET_PING";
static const String CONNECTED_MSG = "SOCKET_CONNECTED";

// Server object, created with the desired port
int port = 80;
Server server(port);

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
  Serial.println("IP: " + ip + ", Port: " + String(port));
  
  server.begin();
  Serial.println("WiFi server initialized!");
}

/**
 * Called whenever a newline-delimited message is received.
 */
void got_message(String msg) {
  Serial.println("[RX] " + msg);
}

/**
 * Send a message to the given client.
 */
void send_message(Client client, String msg) {
  client.println(msg);
  Serial.println("[TX] " + msg);
}

void loop() {
  
  // Looks for a connected client
  Client client = server.available();
  
  if (client) {
    
    // Buffer to store data
    String rx_buffer = "";
    String tx_buffer = "";
    
    // Last time a ping was sent
    int senttime = millis();

    Serial.println("Connected to client.");
    client.println(CONNECTED_MSG);
    
    while (client.connected()) {
      
      // Read data from the client, and when we hit a newline,
      // invoke the message_received callback and echo the message
      // back to the client.
      if(client.available()) {
        while(client.available()) {
          
          char c = client.read();
          
          if(c == '\n') {
            got_message(rx_buffer);
            send_message(client, rx_buffer);
            rx_buffer = "";
            
          } else {
            rx_buffer += c;
          }
        }
      }
      
      // Send messages from Serial to the connected client
      // Delimited by newlines
      if(Serial.available()) {
        while(Serial.available()) {
          
          char c = Serial.read();
          
          if(c == '\n') {
            send_message(client, tx_buffer);
            tx_buffer = "";
          } else tx_buffer += c;
        }
      }
      
      // Send regular ping messages to keep the connection open
      int now = millis();
      if (now - senttime > PING_TIME) {
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
