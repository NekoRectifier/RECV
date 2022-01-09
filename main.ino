#include "Adafruit_NeoPixel.h"

using namespace std;

// 针脚定义 开始
#define MOTOR_A_POS A2
#define MOTOR_A_NEG A3
#define MOTOR_B_POS A4
#define MOTOR_B_NEG A5
#define ENCODER_A 0
#define ENCODER_B 1
#define ENCODER_C 2
#define ENCODER_D 3

#define CRASH_BTN 7

#define LED 5
#define LED_COUNT 8
//占空比电压调制
#define PWMA 11
#define PWMB 13
//针脚定义结束

Adafruit_NeoPixel strip(LED_COUNT, LED, NEO_GRB + NEO_KHZ800);

#define TOTAL_PULSE_WHELL 1560

volatile long encoder_A = 0;
volatile long encoder_B = 0;
volatile long encoder_C = 0;
volatile long encoder_D = 0;
volatile int flag_crash = 0;
unsigned long curr_time, prev_time = 0;
long odometer = 0;
double velocity_A = 0.0;
double velocity_B = 0.0;
int pwm = 80;

// 中断函数内所有的变量在声明时都应该加上volatile属性

void setup()
{
    curr_time = prev_time = millis();
    pinMode(MOTOR_A_POS, OUTPUT);
    pinMode(MOTOR_A_NEG, OUTPUT);
    pinMode(PWMA, OUTPUT);
    pinMode(MOTOR_B_POS, OUTPUT);
    pinMode(MOTOR_B_NEG, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);
    pinMode(ENCODER_C, INPUT);
    pinMode(ENCODER_D, INPUT);
    pinMode(CRASH_BTN, INPUT);

    Serial.begin(9600);

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(MOTOR_A_POS, LOW);
    digitalWrite(MOTOR_A_NEG, HIGH);
    digitalWrite(MOTOR_B_POS, LOW);
    digitalWrite(MOTOR_B_NEG, HIGH);

    prev_time = millis();

    attachInterrupt(digitalPinToInterrupt(0), ISR_enc_A, RISING); //开始中断 函数:timer 模式:RISING
    attachInterrupt(digitalPinToInterrupt(1), ISR_enc_B, RISING);
    attachInterrupt(digitalPinToInterrupt(2), ISR_enc_C, RISING);
    // attachInterrupt(digitalPinToInterrupt(3), ISR_enc_D, RISING);

    attachInterrupt(digitalPinToInterrupt(7), crashDetect, RISING);

    analogWrite(PWMA, pwm); //给定pwm模拟输出
    analogWrite(PWMB, pwm);

    interrupts();
    // RISING 指在引脚由低电平转为高电平时触发
}

void loop()
{
    noInterrupts();
    // speedMeasure();

    if (flag_crash == 1)
    {
        flag_crash == 2; //代表已处理
        digitalWrite(LED_BUILTIN, HIGH);
        odometer = encoder_B;
        // TODO: 此处应对编码器数值做精准化处理
        encoder_B = 0;
        digitalWrite(MOTOR_A_POS, HIGH);
        digitalWrite(MOTOR_A_NEG, LOW);
        digitalWrite(MOTOR_B_POS, HIGH);
        digitalWrite(MOTOR_B_NEG, LOW);
    }

    mileageCheck();
    // curr_time = millis();
    interrupts();
}

void ISR_enc_A()
{
    encoder_A++;
}

void ISR_enc_B()
{
    encoder_B++;
}

void ISR_enc_C()
{
    encoder_C++;
}

// void ISR_enc_D()
// {
//     encoder_D++;
// }

void crashDetect()
{
    flag_crash = 1;
}

void mileageCheck() //用于控制车辆在终点处停下
{
    noInterrupts();
    if (flag_crash == 2)
    {
        if (encoder_B >= odometer)
        {
            digitalWrite(MOTOR_A_POS, LOW);
            digitalWrite(MOTOR_A_NEG, LOW);
            digitalWrite(MOTOR_B_POS, LOW);
            digitalWrite(MOTOR_B_NEG, LOW);
        }
    }
    interrupts();
}

bool speedMeasure() // for high speed
{
    if (curr_time != prev_time)
    {
        long sub_time = curr_time - prev_time;
        int len = (encoder_A / 1560) * 21.0486;
        velocity_A = len / sub_time;
        len = (encoder_C / 1560) * 21.0486;
        velocity_B = len / sub_time;
        prev_time = curr_time;

        encoder_A = encoder_C = 0;

        Serial.print("Left Wheel:");
        Serial.print(velocity_A);
        Serial.print("\t");
        Serial.print("Right Wheel:");
        Serial.println(velocity_B);
    }
}