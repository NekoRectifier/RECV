#include "Adafruit_NeoPixel.h"

using namespace std;

//consants definitions
#define LED_COUNT 8
#define USS_CENTER_OFFSET 0.0 //TODO: Needs to be modified when vehicle is built up.

//pin set
#define LED_pin 6
#define TRIG_pin 13
#define ECHO_pin 11
#define CRASH_pin 10

//global varible init.
volatile double initialDistance = 0.0;
int CRASH_BTN_STATE = 1;
Adafruit_NeoPixel strip(LED_COUNT, LED_pin, NEO_GRB + NEO_KHZ800);

//function pre-declare.
void initComponents();
void initNeoPixel();
void initUltraSonic();
void colorWipe(uint32_t color, int wait);
void flash(uint32_t color, int perTime, int times, int mode);
double getDistance();
void beginTaskSequence();

//main function.
void setup()
{
    Serial.begin(9600);
    initComponents();
    //beginTaskSequence();
}

void loop()
{
}

//main body of sub_functions
void initComponents()
{
    initNeoPixel();
    strip.clear();
    flash(strip.Color(128, 128, 0), 150, 4, 1); //yellow
    //initUltraSonic();
    pinMode(CRASH_pin, INPUT);
    //TODO motors need to be initialized...
}

void initUltraSonic()
{
    pinMode(13, OUTPUT);
    pinMode(11, INPUT);
    delay(50);
    //Show different light pattern correspond to each task
    //flash(strip.Color(0,255,255),50,3); //Blue
    int i = 0;
    double temp = 0.0;
    while (i < 5)
    {
        i++;
        temp = temp + getDistance();
    }

    temp = temp / 5;
    Serial.println(temp);
    if (temp > 100.0 && temp < 2000.0)
    {
        flash(strip.Color(0, 255, 0), 500, 1, 1); //Green
        initialDistance = temp;
    }
    else
    {
        flash(strip.Color(255, 0, 0), 500, 2, 1); //Red
    }
}

void initNeoPixel()
{
    strip.begin();
    colorWipe(strip.Color(127, 127, 127), 150);
    delay(1000);
}

//NeoPixel custom functions
void colorWipe(uint32_t color, int wait)
{
    for (int i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, color);
        strip.show();
        delay(wait);
    }
}

void flash(uint32_t color, int perTime, int times, int mode)
{
    for (int i = 0; i < times; i++)
    {
        strip.setPixelColor(0, color);
        strip.show();
        delay(perTime);

        strip.setPixelColor(0, strip.Color(0, 0, 0));
        strip.show();

        delay(perTime);
        strip.setPixelColor(0, color);
        strip.show();
    }

    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.show();
}

double getDistance()
{

    //Origin merchant instance code, NOT modified yet.

    digitalWrite(13, LOW);
    delayMicroseconds(2);
    digitalWrite(13, HIGH);
    delayMicroseconds(10);
    digitalWrite(13, LOW);
    double distance = pulseIn(11, HIGH) / 58.00;
    delay(10);
    return distance;
    //Outputs maybe in centimeter.
}

void beginTaskSequence()
{
}