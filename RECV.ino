#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "RECV";
const char *passwd = "86309602";

ESP8266WebServer server(80);
//TODO may change server port to 443 soon

uint8 B_LED = LED_BUILTIN;

void setup() {
    digitalWrite(B_LED, HIGH);

    Serial.begin(9600);

    // initiate pin mode
    pinMode(B_LED, OUTPUT); // BUILTIN LED

    WiFi.softAP(ssid, passwd);
    // enable lightings
    
    blink(2, 150);

    Serial.println(WiFi.softAPIP());
    // enable light
    blink(2, 30);

    server.begin();
    server.on("/", handleIndex);
}

void loop() {
    server.handleClient();
}

void blink(int times, int interval) {
    for(int i = 0; i < times; i++) {
        digitalWrite(B_LED, LOW);
        delay(interval);
        digitalWrite(B_LED, HIGH);
        delay(90);
        digitalWrite(B_LED, LOW);
        delay(interval);
        digitalWrite(B_LED, HIGH);
    }
}

void handleIndex() {
    server.send(200, "text/html", "<h1>fuck</h1>");
}
