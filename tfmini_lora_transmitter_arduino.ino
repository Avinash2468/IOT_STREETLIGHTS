#include <SPI.h>
#include <LoRa.h>
#include<SoftwareSerial.h>
SoftwareSerial Serial1(3,4);    //(Rx,Tx)
int dist;
int strength;
int check;
int uart[9];
int i;
boolean x = 0;
const int HEADER = 0x59;
void setup(){
Serial.begin(115200);
Serial1.begin(115200);
  while (!Serial);
  Serial.println("Sensor and LoRa Sender");

  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else
  {
    Serial.println("Started Lora at 433MHz");
  }
}

void loop(){
    x = 0;
    while(!(Serial1.available()));
    while(Serial1.read()!=HEADER);
        uart[0]=HEADER;
    while(Serial1.read()!=HEADER);
        uart[1]=HEADER;
        for(i=2;i<9;i++)
        {
          while(!(Serial1.available()));
          uart[i]=Serial1.read();
        }
         check = uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
         //Serial.print(check&0xff);
         //Serial.print('\t');
         //Serial.print(uart[8]);
         //Serial.print('\n');
         if(uart[8]==(check&0xff))
         {
          dist = uart[2]+uart[3]*256;
          strength = uart[4]+uart[5]*256;
          Serial.print("distance = ");
          Serial.print(dist);
          Serial.print('\t');
          Serial.print("strength = ");
          Serial.println(strength);
          //if(dist<100)
          //{
          x = 1;
          transmitter();
          //}
          //transmitter();
         }
}


void transmitter() {
  Serial.print("Sending packet: ");
  LoRa.beginPacket();
  //LoRa.print("distance = ");
  LoRa.print(dist);
  //LoRa.print('\t');
  //LoRa.print("strength = ");
  //LoRa.print(strength);
  LoRa.endPacket();
  delay(10);
}
