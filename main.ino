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
void flash(uint32_t color,int perTime, int times);
double getDistance();
void beginTaskSequence();

//main function.
void setup()
{
    Serial.begin(9600);
    delay(100);
    initComponents();

    strip.clear();
    flash(strip.Color(255,100,100),200,2);
    delay(100);
    beginTaskSequence();
}

void loop()
{
    //Serial.println(getDistance());
}


//main body of sub_functions
void initComponents() 
{
    initNeoPixel();
    //flash(strip.Color(128,128,0),150,4);
    initUltraSonic();
    pinMode(CRASH_pin, INPUT);
    //TODO motors need to be initialized...
}

void initUltraSonic()
{
    pinMode(13, OUTPUT);
    pinMode(11, INPUT);
    delay(200);
    //TODO: Show different light pattern correspond to each task
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    delay(700);
    Serial.println(getDistance());
    
}

void initNeoPixel()
{
    //an harmless supportment code for Adafruit Trinket 5V 16 MHz.
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif

    strip.begin();
    strip.clear();   //Nya?

    //strip.setBrightness(255);

    colorWipe(strip.Color(127,127,127),50);
    delay(5000);
    strip.clear();

}

//NeoPixel custom functions
void colorWipe(uint32_t color, int wait)
{
    for(int i=0; i<strip.numPixels(); i++)
    {
        strip.setPixelColor(i, color);
        strip.show();
        delay(wait);
    }
}

void flash(uint32_t color,int perTime, int times)
{

    for(int j = 0; j < times; j++)
    {
        strip.setPixelColor(0, color);
        strip.setPixelColor(1, color);
        strip.setPixelColor(6, color);
        strip.setPixelColor(7, color);
        strip.show();
        delay(perTime);
        strip.clear();
    }
    strip.clear();
    
}

double getDistance()
{

    //Origin merchant instance code, NOT modified yet.

    digitalWrite(13, LOW);
    delayMicroseconds(13);
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

    if (total > 320.0 && total < 400.0) {
        flash(strip.Color(255,255,255),100,2);
    } else if(total > 240.0 && total < 320.0) {
        flash(strip.Color(255,255,255),100,2);
    } else if(total > 140.0 && total < 240.0) {
        flash(strip.Color(255,255,255),100,2);
    } else {
    }

    //motor.start();
    
}