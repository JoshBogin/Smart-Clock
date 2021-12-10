#include <WiFiNINA.h>

char SSID[] = "ChamplainPSK";   // the name of your network/HotSpot
char PASS[] = "letusdare";  // the password of your WiFi

// MQTT settings
const String ID = "smart";
const String BROKER = "broker.hivemq.com";
const String CLI_NAME = ID + "clock";
const String TIME_TOPIC = "t/time";
const String ALARM_TOPIC = "t/alarm";
const String MESSAGE_TOPIC = "t/message";