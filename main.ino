#define MOTOR_1 9
#define MOTOR_2 10
#define PWMA 11      //PWM analog?
#define ENCODER_A 2  //reading
#define ENCODER_B 3

volatile long encoder_A;
volatile long encoder_B;

unsigned long curr_time, prev_time = 0;

double velocity = 0.0;

int pwm = 70;

// 中断函数内所有的变量在声明时都应该加上volatile属性
// Arduino Uno 仅支持2,3针脚作为中断引脚

void setup()
{
    pinMode(MOTOR_1, OUTPUT);
    pinMode(MOTOR_2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);

    Serial.begin(9600);

    digitalWrite(MOTOR_1, HIGH);
    digitalWrite(MOTOR_2, LOW);
    prev_time = millis();

    attachInterrupt(0, ISR_enc_A, RISING);  //开始中断 函数:timer 模式:RISING
    attachInterrupt(1, ISR_enc_B, RISING);
    interrupts();
    // RISING 指在引脚由低电平转为高电平时触发
}

void loop()
{
    //设定旋转方向

    //给定pwm模拟输出
    analogWrite(PWMA, pwm);

    noInterrupts();
    Serial.print("ENC_LEFT: ");
    Serial.println(readEncoderValue(0));

    Serial.print("ENC_RIGHT: ");
    Serial.println(readEncoderValue(1));

    speedMeasure();
    Serial.print("Velocity: ");
    Serial.println(velocity);

    delay(100);
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

long readEncoderValue(int seletor)
{
    noInterrupts();
    long curr;

    if (seletor == 0)
    {
        curr = encoder_A;
    }
    else
    {
        curr = encoder_B;
    }
    interrupts();
    return curr;
}

bool speedMeasure()  //for high speed
{
    curr_time = millis();
    noInterrupts();

    unsigned long total_encode = encoder_A + encoder_B;

    if (abs(curr_time - prev_time) >= 100)
    {
        velocity = total_encode / (1560 * 0.1);

        encoder_A = encoder_B = 0;
        return true;
    }
    else
    {
        prev_time = curr_time;
    }
    interrupts();
}
