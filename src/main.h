#include <WiFiNINA.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char THING_ID[] = "d2377de8-03b2-41cb-bb2f-86fde15df575";

char SSID[] = "Wes";   // the name of your network/HotSpot
char PASS[] = "passedword";  // the password of your WiFi

// MQTT settings
const String ID = "smart";
const String BROKER = "broker.hivemq.com";
const String CLI_NAME = ID + "clock";
const String TOPIC = "t/time";
//const String INTERVAL_TOPIC = "josh/interval";

float temperature;

void initProperties() {
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(temperature, READ, 1 * SECONDS, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);