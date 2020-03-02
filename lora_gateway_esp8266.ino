#include <SPI.h>
#include <LoRa.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
const char* apiKey = "1ZRYFKBEB45KZJEF";       // Enter your Write API key from ThingSpeak
unsigned long int ch = 1007029;
const char* ssid = "K99";           // Give your wifi network name
const char* password = "Karan@221299";// Give your wifi network password
const char* server = "api.thingspeak.com";
String LoRaData;
//define the pins used by the transceiver module
#define ss 15
#define rst 16
#define dio0 4

WiFiClient client;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, password);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  //LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  ThingSpeak.begin(client); 
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      LoRaData = LoRa.read();
      Serial.print(LoRaData); 
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
    
    if (!LoRaData) 
                 {
                     Serial.println("Failed to get data from LoRa!");
                      return;
                 }

                 Serial.print("Vehicle/Pedestrian detection: ");
                             Serial.print(LoRaData);
                        
                             Serial.println("%. Send to Thingspeak.");
                 int x = ThingSpeak.writeField(ch, 1, 69696969, apiKey);
                 
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
                   
                         /*if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(LoRaData);
                             postStr += "\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Vehicle/Pedestrian detection: ");
                             Serial.print(LoRaData);
                        
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 */
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);

  
}
