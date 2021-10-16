#include <Adafruit_NeoPixel.h>

using namespace std;

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

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
void flash(uint32_t color, int perTime, int times);
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
    flash(strip.Color(128, 128, 0), 150, 4); //yellow
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
        flash(strip.Color(0, 255, 0), 100, 1); //Green
        initialDistance = temp;
    }
    else
    {
        flash(strip.Color(255, 0, 0), 100, 2); //Red
    }
}

void initNeoPixel()
{
//an harmless supportment code for Adafruit Trinket 5V 16 MHz.
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

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

void flash(uint32_t color, int perTime, int times)
{

    /* for(int j = 0; j < times; j++)
    {
        strip.setPixelColor(0, color);
        strip.setPixelColor(1, color);
        strip.setPixelColor(6, color);
        strip.setPixelColor(7, color);
        strip.show();
        delay(perTime);
        
        
        strip.clear();
    }

    strip.clear(); */

    strip.setPixelColor(0, color);
    strip.show();
    delay(1000);
    strip.clear();
    delay(1000);
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
    //if ultrasonic module is fine...
    initialDistance = getDistance();
    double total = initialDistance + USS_CENTER_OFFSET;

    if (total > 320.0 && total < 400.0)
    {
        flash(strip.Color(255, 255, 255), 100, 2);
    }
    else if (total > 240.0 && total < 320.0)
    {
        flash(strip.Color(255, 255, 255), 100, 2);
    }
    else if (total > 140.0 && total < 240.0)
    {
        flash(strip.Color(255, 255, 255), 100, 2);
    }
    else
    {
    }

    //motor.start();
}