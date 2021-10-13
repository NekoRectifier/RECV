#define TRIG_pin 13;
#define ECHO_pin 11;

#define CRASH_BTN 10;

volatile double initialDistance = 0.0;

void initComponents();
void flash(int pattern);

void setup()
{
    Serial.begin(9600);
    delay(100);
    initComponents();

    flash(0);
    Serial.print(USDevice.distance());
    
}

void loop()
{

}

void initComponents() 
{
    flash(10000);
    initUltraSonic();

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

void flash(int pattern)
{
    switch(i)
    {
        case 0:
            YELLOW_FLASH_x2
            break;
        case 10000:
            
            break;
        default:
        break;
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