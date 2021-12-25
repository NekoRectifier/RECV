#include "Adafruit_NeoPixel.h"

using namespace std;

#define MOTOR_1 9
#define MOTOR_2 10
#define PWMA 11  //占空比电压调制
#define ENCODER_A 2
#define ENCODER_B 3
#define CRASH_BTN [?]

#define LED 5
#define LED_COUNT 8
Adafruit_NeoPixel strip(LED_COUNT, LED, NEO_GRB + NEO_KHZ800);

#define TOTAL_PULSE_WHELL 1560

volatile long encoder_A;
volatile long encoder_B;
volatile int flag_crash = 0;

unsigned long curr_time, prev_time = 0;
long odometer = 0;

double velocity = 0.0;

int pwm = 75;  //固定为该数值 正好

// 中断函数内所有的变量在声明时都应该加上volatile属性
// Arduino Uno 仅支持2,3针脚作为中断引脚

void setup()
{
    pinMode(MOTOR_1, OUTPUT);
    pinMode(MOTOR_2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    //pinMode(LED, OUTPUT);

    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);
    pinMode(CRASH_BTN, INPUT);

    Serial.begin(9600);
    //initLED();

    digitalWrite(MOTOR_1, LOW);
    digitalWrite(MOTOR_2, HIGH);

    prev_time = millis();

    attachInterrupt(0, ISR_enc_A, CHANGE);  //开始中断 函数:timer 模式:RISING
    //attachInterrupt(1, ISR_enc_B, CHANGE);
    attachInterrupt(1), crashDetect, CHANGE);

    // 由于Arduino有且仅有两个中断针脚, 故只采用编码器的A段信息输入

    analogWrite(PWMA, pwm);  //给定pwm模拟输出

    interrupts();
    // RISING 指在引脚由低电平转为高电平时触发
}

void loop()
{
    noInterrupts();
    // Serial.print("ENC_LEFT: ");
    // Serial.println(readEncoderValue(0));

    // Serial.print("ENC_RIGHT: ");
    // Serial.println(readEncoderValue(1));

    if (flag_crash == 1)
    {
        flag_crash == 2;
        odometer = encoder_A;
        encoder_A = 0;
        digitalWrite(MOTOR_1, HIGH);
        digitalWrite(MOTOR_2, LOW);
    }

    mileageCheck();

    interrupts();
}

void colorWipe(uint32_t color, int wait)
{
    for (int i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, color);
        strip.show();
        delay(wait);
    }
}

void initLED()
{
    strip.begin();
    colorWipe(strip.Color(255, 255, 24), 5000);
    strip.clear();
}

void ISR_enc_A()
{
    encoder_A++;
}

// void ISR_enc_B()
// {
//     encoder_B++;
// }

void crashDetect()
{
    flag_crash = 1;
}

void mileageCheck()
{
    noInterrupts();
    if (flag_crash = 2)
    {
        if (encoder_A >= odometer) {
            digitalWrite(MOTOR_1, LOW);
            digitalWrite(MOTOR_2, LOW);
        }
    }
    interrupts();
}

// bool speedMeasure()  //for high speed
// {
//     curr_time = millis();
//     noInterrupts();

//     unsigned long total_encode = encoder_A + encoder_B;

//     if (abs(curr_time - prev_time) >= 100)
//     {
//         velocity = total_encode / (1560 * 0.1);

//         encoder_A = encoder_B = 0;
//         return true;
//     }
//     else
//     {
//         prev_time = curr_time;
//     }
//     interrupts();
// }
