#include "Adafruit_NeoPixel.h"

using namespace std;

// 针脚定义 开始
#define MOTOR_A_POS 9
#define MOTOR_A_NEG 10
#define MOTOR_B_POS 9
#define MOTOR_B_NEG 10
#define ENCODER_A 2
#define ENCODER_B 3
#define ENCODER_C 
#define ENCODER_D 
#define CRASH_BTN [?]
#define LED 5
#define LED_COUNT 8
//占空比电压调制
#define PWMA 11  
#define PWMB 11
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
double velocity = 0.0;
int pwm = 75;

// 中断函数内所有的变量在声明时都应该加上volatile属性

void setup()
{
    pinMode(MOTOR_A_POS, OUTPUT);
    pinMode(MOTOR_A_NEG, OUTPUT);
    pinMode(PWMA, OUTPUT);
    pinMode(MOTOR_B_POS, OUTPUT);
    pinMode(MOTOR_B_NEG, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);
    pinMode(ENCODER_C, INPUT);
    pinMode(ENCODER_D, INPUT);
    pinMode(CRASH_BTN, INPUT);
    pinMode(LED_BULITIN, OUTPUT);

    Serial.begin(9600);

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(MOTOR_A_POS, LOW);
    digitalWrite(MOTOR_A_NEG, HIGH);
    digitalWrite(MOTOR_B_POS, LOW);
    digitalWrite(MOTOR_B_NEG, HIGH);

    prev_time = millis();

    attachInterrupt(digitalPinToInterrupt(0), ISR_enc_A, CHANGE);  //开始中断 函数:timer 模式:RISING
    attachInterrupt(digitalPinToInterrupt(1), ISR_enc_B, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(pin), function, mode);
    //attachInterrupt(digitalPinToInterrupt(pin), function, mode);
    
    attachInterrupt(digitalPinToInterrupt(7), crashDetect, CHANGE);

    analogWrite(PWMA, pwm);  //给定pwm模拟输出
    analogWrite(PWMB, pwm);

    interrupts();
    // RISING 指在引脚由低电平转为高电平时触发
}

void loop()
{
    noInterrupts();

    if (flag_crash == 1)
    {
        flag_crash == 2;                    //代表已处理
        digitalWrite(LED_BUILTIN, HIGH);
        odometer = encoder_A;
        //TODO: 此处应对编码器数值做精准化处理
        encoder_A = 0;
        digitalWrite(MOTOR_A_POS, HIGH);
        digitalWrite(MOTOR_A_NEG, LOW);
    }

    mileageCheck();

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

void ISR_enc_D()
{
    encoder_D++;
}

void crashDetect()
{
    flag_crash = 1;
}

void mileageCheck()                         //用于控制车辆在终点处停下
{
    noInterrupts();
    if (flag_crash = 2)
    {
        if (encoder_A >= odometer) {
            digitalWrite(MOTOR_A_POS, LOW);
            digitalWrite(MOTOR_A_NEG, LOW);
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
