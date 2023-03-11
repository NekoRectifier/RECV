#include <ESP8266WiFi.h>

const char *ssid = "RECV";
const char *passwd = "86309602";

void setup() {
    Serial.begin(9600);

    WiFi.softAP(ssid, passwd);

    Serial.println(WiFi.softAPIP());
}

void loop() {

}
