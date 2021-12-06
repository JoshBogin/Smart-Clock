#include <WiFiNINA.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

char SSID[] = "Wes";   // the name of your network/HotSpot
char PASS[] = "passedword";  // the password of your WiFi

// MQTT settings
const String ID = "smart";
const String BROKER = "broker.hivemq.com";
const String CLI_NAME = ID + "clock";
const String TOPIC = "t/time";