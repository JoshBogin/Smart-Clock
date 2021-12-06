#include <Arduino.h>
#include <SNU.h>
#include "main.h"
#include <PubSubClient.h>
#include <iostream>
#include <string>

int pinA = 0;
int pinB = 1;
int pinC = 2;
int pinD = 3;
int pinE = 4;
int pinF = 5;
int pinG = 6;
int pinDP = 7;

int D1 = 8;
int D2 = 9;
int D3 = 10;
int D4 = 11;
int digits[4] = {D1, D2, D3, D4};

boolean duiz = false;
boolean hon = false;
boolean dot = false;

// time vars
int hours = 8;
//int hr_dig_1 = 8;
int minutes = 46;
//int min_dig_1 = 6;

#define Delay 1000

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Set IFTTT Webhooks event name and key
#define IFTTT_KEY "cp9mCVLbwEFBof76xY6Sm"
#define IFTTT_EVENT1 "temp_too_hot"
#define IFTTT_EVENT2 "temp_too_cold"
#define IFTTT_EVENT3 "air_too_humid"
#define IFTTT_EVENT4 "air_too_dry"

int interval = 5000;
int threshold = 5000;
int time_now = 0;
unsigned long lastInterval = 0;
String intervalStr = "60";
bool showTempNotHum = true;

void connectWiFi()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi..");
    WiFi.begin(SSID, PASS);
    delay(500);
  }

  Serial.println("Connected!");
  Serial.println(WiFi.localIP());
}

void clientCallback(char *topic, uint8_t *payload, unsigned int length)
{
  Serial.print("callback\n");
  char buff[length + (unsigned int)1];
  for (int i = 0; i < length; i++)
  {
    buff[i] = (char)payload[i];
  }
  buff[length] = '\0';

  Serial.print(*buff);
  
  //char * returns[length] = { strtok(*buff, ":") };
  //Serial.print("\nreturns: ");
  //Serial.println(*returns);
  int hrs = int(buff[0] + buff[1]);
  int mins = int(buff[2] + buff[3]);
  Serial.print("\nhrs: ");
  Serial.println(hrs);
  Serial.print("\nmins: ");
  Serial.println(mins);
  hours = hrs;
  minutes = mins;
}

void reconnectMQTTClient() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection:");

    if (client.connect(CLI_NAME.c_str())) {
      Serial.println("Connected");
      client.subscribe(TOPIC.c_str());
    } else {
      Serial.print("Connection Failed! Retrying in 10 seconds...");
      Serial.println(client.state());
      delay(10000);
    }
  }
}

void createMQTTClient()
{
  client.setServer(BROKER.c_str(), 1883);
  client.setCallback(clientCallback);
  reconnectMQTTClient();
}

// function to set a light to a given number
void lightNumber(int numberToDisplay, boolean isDot) {
  switch (numberToDisplay) {
    case 0:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, LOW);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 1:
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 2:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, LOW);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, HIGH);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 3:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, HIGH);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 4:
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 5:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, LOW);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 6:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, LOW);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 7:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 8:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    case 9:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      if (isDot) {
        digitalWrite(pinDP, HIGH);
      } else {
        digitalWrite(pinDP, LOW);
      }
      break;

    // turn off all segments
    case -1:
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, LOW);
      digitalWrite(pinC, LOW);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      break;
  }
}

/*
void num_loop() {
  for( int i = 0; i <= 9999; i++){
    duiz = false;
    hon = false;
    for (int x = 0; x < 50; x++) {
      int figur = i;
      for(int j = 0; j < 4; j++) {
        //Turn on a digit for a short amount of time
        switch(j) {
          case 1:
            if (figur > 999) {
              digitalWrite(digits[j], LOW);
              lightNumber(figur / 1000);            // for example 2511 / 1000 = 2
              figur %= 1000;                        // new value of figur = 511         figur = figur %1000
              
              delayMicroseconds(Delay); 
              if (figur < 100){
                duiz = true;
                if (figur < 10){
                  hon = true; 
                }
              } else {
                duiz = false;
              }
            }
            break;
          case 2:
            if (duiz == true) {
              digitalWrite(digits[j], LOW);
              lightNumber(0);
              delayMicroseconds(Delay);
            } if (hon == true) {
              break;
            }
          
            if (figur > 99 && figur < 1000){
              digitalWrite(digits[j], LOW);
              lightNumber(figur / 100);
              figur %= 100;
              delayMicroseconds(Delay); 
              if (figur < 10){
              hon = true;
              } else {
                hon = false;
              }
            }
            break;
          case 3:
            if(hon == true){
              digitalWrite(digits[j], LOW);
              lightNumber(0);
              delayMicroseconds(Delay);
              break;
            }
          
            if(figur > 9 && figur < 100){
              digitalWrite(digits[j], LOW);
              lightNumber(figur / 10); 
              figur %= 10;
              delayMicroseconds(Delay); 
            }
            break;

          case 4:
            if (figur < 10) {
              digitalWrite(digits[j], LOW);
              lightNumber(figur); 
              delayMicroseconds(Delay); 
              break;
            }
            break;
        }
        //Turn off all segments
        lightNumber(-1); 

        //Turn off all digits
        digitalWrite(digits[0], HIGH);
        digitalWrite(digits[1], HIGH);
        digitalWrite(digits[2], HIGH);
        digitalWrite(digits[3], HIGH);
      }
      delayMicroseconds(1500);
    }
  }
}
*/

void setup() {
  Serial.begin(9600);

    // Defined in IFTTTWebhookMKG.h
  initProperties();

  while (!Serial)
    Serial.print("."); // Wait for Serial to be ready

  connectWiFi();
  createMQTTClient();

  // initialize the digital pins as outputs.
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);
  pinMode(pinDP, OUTPUT);
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);
}

void loop() {
  reconnectMQTTClient();
  client.loop();

  for(int j = 1; j <= 4; j++) {
    //Turn on a digit for a short amount of time
    //Serial.print(j);
    switch(j) {
      case 1:
        dot = false;
        digitalWrite(D1, LOW);

        if(hours >= 10) {
          lightNumber(hours / 10, dot);
        } else {
          lightNumber(0, dot);
        }
        delayMicroseconds(Delay);
        break;
      case 2:
        digitalWrite(D2, LOW);
        dot = true;
        lightNumber(hours % 10, dot);
        delayMicroseconds(Delay); 
        break;
      case 3:
        dot = false;
        digitalWrite(D3, LOW);
        
        if(minutes >= 10) {
          lightNumber(minutes / 10, dot);
        } else {
          lightNumber(0, dot);
        }
        delayMicroseconds(Delay); 
        break;
      case 4:
        dot = false;
        digitalWrite(D4, LOW);

        lightNumber(minutes % 10, dot);
        delayMicroseconds(Delay); 
        break;
    }
    //Turn off all segments
    lightNumber(-1, dot);

    //Turn off all digits
    digitalWrite(digits[0], HIGH);
    digitalWrite(digits[1], HIGH);
    digitalWrite(digits[2], HIGH);
    digitalWrite(digits[3], HIGH);
  }
  delayMicroseconds(Delay); 
}