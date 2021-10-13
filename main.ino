#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
    #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_COUNT 8

//pin set
#define LED_pin 6
#define TRIG_pin 13
#define ECHO_pin 11
#define CRASH_BTN 10

//global varible init.
volatile double initialDistance = 0.0;
Adafruit_NeoPixel strip(LED_COUNT, LED_pin, NEO_GRB, NEO_KHZ800);

//function pre-declare.
void initComponents();
void initNeoPixel();
void initUltraSonic();
void flash(int pattern);


//main function.
void setup()
{
    Serial.begin(9600);
    delay(100);
    initComponents();

    flash(0);
    
}

void loop()
{

}


//main body of sub_functions
void initComponents() 
{
    flash(10000);
    initUltraSonic();
    initNeoPixel();
}

void initUltraSonic()
{
    pinMode(2, OUTPUT);
    pinMode(3, INPUT);
    delay(200);
    initialDistance =  getDistance();
    //TODO: Show different light pattern correspond to each task
    Serial.println(initialDistance);
}

void initNeoPixel()
{
    //an harmless supportment code for Adafruit Trinket 5V 16 MHz.
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif

    strip.begin();
    strip.stop();   //Nya?

    //strip.setBrightness(255);

    colorWipe(strip.Color(127,127,127),50);
    delay(5000);
    strip.stop();

}

void flash(int pattern)
{
    switch(i)
    {
        case 0: //Car is ready
            flash(strip.Color(0,128,0),200,2,{0,1,6,7});
            break;
        case -1:
            flash(strip.Color(255,0,0),1000,3,{0,2,5,7});
            break;
        case 10000:
            break;
        default:
            break;
    }
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

void flash(uint32_t color,int perTime, int times, int[] pixels)
{
    for(int j = 0; j < times; j++)
    {
        for(int& i : pixels)
        {
            strip.setPixelColor(i,color);
        }
        strip.show();
        delay(perTime);
        strip.clear();
    }
    
}

double getDistance()
{

    //Origin merchant instance code, NOT modified yet.

    digitalWrite(2, LOW);
    delayMicroseconds(2);
    digitalWrite(2, HIGH);
    delayMicroseconds(10);
    digitalWrite(2, LOW);
    double distance = pulseIn(3, HIGH) / 58.00;
    delay(10);
    return distance;
}