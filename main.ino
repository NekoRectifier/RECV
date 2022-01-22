#include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>
#include "index.h"

//#define MOTOR_A_POS 3
//#define MOTOR_A_NEG D3
//#define MOTOR_B_POS 1
//#define MOTOR_B_NEG D8
// (始终可用情况)

// 针脚定义 开始
#define MOTOR_A_POS 3
#define MOTOR_A_NEG D3
#define MOTOR_B_POS 1
#define MOTOR_B_NEG D8
// 不要改动! GPIO16 不能正常输出
#define ENCODER_A D1
//#define ENCODER_B D1
#define ENCODER_C D2
//#define ENCODER_D D3
#define CRASH_BTN D7

//占空比电压调制
#define PWMA D5
#define PWMB D6
//针脚定义结束

#define TOTAL_PULSE_WHELL 1560
#define C 20.7345

volatile long encoder_A = 0;
volatile long encoder_C = 0;

long temp_ena, temp_enc = 0;

long prev_A, prev_C = 0;
volatile long odometer = 0;
double velocity_A = 0.0;
double velocity_B = 0.0;
short pwm_A = 96;  // now correspond to 'C'
short pwm_B = 100; // now correspond to 'A'
int cycle = 0;
volatile int flag = 0;
int start_flag = -1;

unsigned long curr, prev = 0;

// 中断函数内所有的变量在声明时都应该加上volatile属性

ESP8266WiFiMulti wifi;
ESP8266WebServer server(80);

void setup()
{
	// Serial.begin(115200);

	// delay(3000);

	pinMode(MOTOR_A_POS, OUTPUT);
	pinMode(MOTOR_A_NEG, OUTPUT);
	pinMode(PWMA, OUTPUT);
	pinMode(MOTOR_B_POS, OUTPUT);
	pinMode(MOTOR_B_NEG, OUTPUT);
	pinMode(PWMB, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	pinMode(ENCODER_A, INPUT);
	// pinMode(ENCODER_B, INPUT);
	pinMode(ENCODER_C, INPUT);
	// pinMode(ENCODER_D, INPUT);
	pinMode(CRASH_BTN, INPUT);

	analogWrite(PWMA, pwm_A); //给定pwm模拟输出
	analogWrite(PWMB, pwm_B);

	wifi.addAP("HYX_WLAN", "86309602");
	while (wifi.run() != WL_CONNECTED)
	{
		delay(150);
	}
	server.on("/", handleRoot);
	server.onNotFound(handleNotFound);
	server.on("/update_varible", handleupdate_varible);
	server.on("/update_speed", handleupdate_speed);
	server.on("/act", handleact);
	server.begin();

	while (start_flag == -1)
	{
		server.handleClient();
		delay(50);
	}

	attachInterrupt(digitalPinToInterrupt(ENCODER_A), ISR_enc_A, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENCODER_C), ISR_enc_C, CHANGE);
	attachInterrupt(digitalPinToInterrupt(CRASH_BTN), crashDetect, RISING);

	digitalWrite(MOTOR_A_POS, HIGH);
	digitalWrite(MOTOR_A_NEG, LOW);
	digitalWrite(MOTOR_B_POS, HIGH);
	digitalWrite(MOTOR_B_NEG, LOW);

	curr = prev = millis();
}

void loop()
{
	server.handleClient();

	speedDetect();

	long avg = ((encoder_A + encoder_C) / 2) - odometer;
	// 注意计算方式

	if (avg > odometer)
	{
		digitalWrite(MOTOR_A_NEG, LOW);
		digitalWrite(MOTOR_B_NEG, LOW);
	}
}

void handleRoot()
{ //处理网站根目录“/”的访问请求
	String html = webpage;
	server.send(200, "text/html", html); // NodeMCU将调用此函数。
}

void handleNotFound()
{													  // 当浏览器请求的网络资源无法在服务器找到时，
	server.send(404, "text/plain", "404: Not found"); // NodeMCU将调用此函数。
}

void handleupdate_varible()
{
	server.send(200, "text/json", "{ \"ena\":" + String(encoder_A) + ", \"enc\":" + String(encoder_C) + ", \"odometer\":" + String(odometer) + '}');
	// 在WebConsole里, enc对应enl(左) ena对于enr(右) 就是反过来的
}

void handleact()
{
	String argument = server.arg("selector");
	if (argument == "0")
	{
		start_flag = 0;
		server.send(200, "text/plain", "ok");
	}
	else if (argument == "1")
	{
		encoder_A = encoder_C = 0;
		odometer = 0;
		server.send(200, "text/plain", "ok");
	}
}

void handleupdate_speed() // TODO 左右测试
{
	server.send(200, "text/json", "{ \"left\":" + String(velocity_B) + ", \"right\":" + String(velocity_A) + " }");
}

ICACHE_RAM_ATTR void ISR_enc_A()
{
	encoder_A++;
}

ICACHE_RAM_ATTR void ISR_enc_C()
{
	encoder_C++;
}

ICACHE_RAM_ATTR void crashDetect()
{
	digitalWrite(MOTOR_A_POS, LOW);
	digitalWrite(MOTOR_A_NEG, HIGH);
	digitalWrite(MOTOR_B_POS, LOW);
	digitalWrite(MOTOR_B_NEG, HIGH);

	digitalWrite(LED_BUILTIN, HIGH);
	flag = 1;
	odometer = (encoder_A + encoder_C) / 2;
}

void speedDetect()
{
	curr = millis();
	if (curr - prev >= 1000)
	{

		// A as left...
		velocity_A = ((encoder_A - temp_ena) / 3120) * C;
		velocity_B = ((encoder_C - temp_enc) / 3120) * C;

		temp_ena = encoder_A;
		temp_enc = encoder_C;
		prev = millis();
	}
	// 取得的转速单位为 转/秒
}

void speedAdjust() // for high speed
{
	u32 diff_A = encoder_A - prev_A;
	u32 diff_C = encoder_C - prev_C;

	velocity_A = diff_A / (3120);
	velocity_B = diff_C / (3210);

	if (velocity_A > velocity_B)
	{
		pwm_A++;
		pwm_B--;
	}
	else
	{
		pwm_A--;
		pwm_B++;
	}
	analogWrite(PWMA, pwm_A);
	analogWrite(PWMB, pwm_B);

	prev_A = encoder_A;
	prev_C = encoder_C;
}