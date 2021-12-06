#include <Arduino.h>
#include "main.h"
#include <PubSubClient.h>

using namespace std;

const int pinA = 0;
const int pinB = 1;
const int pinC = 2;
const int pinD = 3;
const int pinE = 4;
const int pinF = 5;
const int pinG = 6;
const int Button = 7;

const int D1 = 8;
const int D2 = 9;
const int D3 = 10;
const int D4 = 11;
int digits[4] = {D1, D2, D3, D4};

// alarm and disarm button
const int Buzzer = 12;
const int pinDP = A6;
const int Light = A5;
int buttonVal;

boolean dot = false;

// current time vars
int hours = 0;
int minutes = 10;
int seconds = 0;
unsigned long lastMinute = 0;
boolean validateSeconds = false;
// alarm time vars
boolean alarm_set = false;
int alarm_hours = 0;
int alarm_minutes = 8;

#define Delay 1000

WiFiClient wifiClient;
PubSubClient client(wifiClient);

int time_now = 0;
unsigned long lastInterval = 0;

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
  String rec_topic = String(topic);

  char buff[length + (unsigned int)1];

  for (int i = 0; (unsigned int)i < length; i++)
  {
    buff[i] = (char)payload[i];
  }
  buff[length] = '\0';

  Serial.print(buff);
  Serial.print("\n");

  String message = buff;
  String hrs = message.substring(0, 2);
  String mins = message.substring(3, 5);
  String secs = message.substring(6, 8);

  Serial.print(rec_topic + "\n");

  if (rec_topic == "t/time") {
    hours = hrs.toInt();
    minutes = mins.toInt();
    seconds = secs.toInt();
    lastMinute = millis();
    validateSeconds = true;
  } else {
    alarm_hours = hrs.toInt();
    alarm_minutes = mins.toInt();
    Serial.print("Alarm Set!\n");
    Serial.print(String(alarm_hours) + ":" + String(alarm_minutes) + "\n");
    alarm_set = true;
  }
}

void reconnectMQTTClient() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection:");

    if (client.connect(CLI_NAME.c_str())) {
      Serial.println("Connected");
      client.subscribe(TIME_TOPIC.c_str());
      client.subscribe(ALARM_TOPIC.c_str());
    } else {
      Serial.print("Connection Failed! Retrying in 5 seconds...");
      Serial.println(client.state());
      delay(5000);
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

void setup() {
  Serial.begin(9600);

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
  pinMode(Buzzer, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  pinMode(Light, OUTPUT);
  digitalWrite(Light, LOW);
  noTone(Buzzer);
}

void loop() {
  reconnectMQTTClient();
  client.loop();

  if ((validateSeconds && millis() >= lastMinute + ((60 - seconds) * 1000)) || (millis() >= lastMinute + 60000)) {
    minutes++;

    if (minutes >= 60) {
      minutes = 0;
      hours++;
    }
    if (hours >= 23) {
      hours = 0;
      minutes = 0;
    }
    lastMinute = millis();
    validateSeconds = false;
  }

  if (alarm_set) {
    if (alarm_hours <= hours && alarm_minutes <= minutes) {
      Serial.print("ALARM ALARM WAKE UP MOTHERFUCKER\n");
      tone(Buzzer, 7000);
      digitalWrite(Light, HIGH);
      // maybe add something here to make the clock digits flicker on and off
      // would just be turning off all digits and then delaying and then the loop would turn them back on
      alarm_set = false;
    }
  }

  buttonVal = digitalRead(Button);
  if (buttonVal == LOW) {
    Serial.print("Alarm disarmed\n");
    noTone(Buzzer);
    digitalWrite(Light, LOW);
  }
  
  for(int j = 1; j <= 4; j++) {
    //Turn on a digit for a short amount of time
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