#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "static_site.h"

const char *ssid = "RECV";
const char *passwd = "86309602";

ESP8266WebServer server(80);
//TODO may change server port to 443 soon

// pin mode definition
uint8 A_LED = LED_BUILTIN;
uint8 B_LED = LED_BUILTIN_AUX;
uint8 PWM = D2;
uint8 IN2 = D3;
uint8 IN1 = D4;

uint8 ENC_A = D5;
uint8 ENC_B = D6;

void setup() {
    // supress led from enabling
    digitalWrite(A_LED, HIGH); // high means off (lights)
    digitalWrite(B_LED, HIGH);

    Serial.begin(9600);

    // initiate pin mode
    pinMode(A_LED, OUTPUT); // BUILTIN LED
    pinMode(B_LED, OUTPUT); // BUILTIN LED
    attachInterrupt(ENC_B, intterupt_B, CHANGE);
    attachInterrupt(ENC_A, intterupt_A, CHANGE);

    WiFi.softAP(ssid, passwd);
    // enable lightings
    
    blink(2, 90, A_LED);

    Serial.println(WiFi.softAPIP());
    // enable light
    blink(2, 110, B_LED);

    //mechanics init...



    server.on("/", handleIndex);
    server.on("/heartbeat", handleHeartbeat);

    server.begin();
    blink(3, 200, A_LED);
}

void loop() {
    server.handleClient();
}

void blink(int times, int interval, uint led) {

    /* 
        interval => time of turned ON
    */
    
    delay(70);
    for (int i = 0; i < times; i++) {
        digitalWrite(led, LOW);
        delay(interval);
        digitalWrite(led, HIGH);
        delay(70);
    }
}

ICACHE_RAM_ATTR void intterupt_A() {
    Serial.println("INTR A");
}

ICACHE_RAM_ATTR void intterupt_B() {
    Serial.println("INTR B");
}

void handleIndex() {
    server.send(200, "text/html", static_site);
}

void handleHeartbeat() {
    server.send(200, "text/plain", "beat_ack");
}
