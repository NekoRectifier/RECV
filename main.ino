#include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "index.h"

//#define MOTOR_A_POS 3
//#define MOTOR_A_NEG D3
//#define MOTOR_B_POS 1
//#define MOTOR_B_NEG D8
// (始终可用情况)

#define MOTOR_A_POS 3
#define MOTOR_A_NEG D3
#define MOTOR_B_POS 1
#define MOTOR_B_NEG D8
// 不要改动! GPIO16 不能正常输出
#define ENCODER_A D1
#define ENCODER_C D2
#define CRASH_BTN D7

#define PWMA D5
#define PWMB D6

#define TOTAL_PULSE_WHEEL 1560 * 2
#define C 20.7345
#define DECELERATION_THRESOLD 57180 // 255cm
// 不同车道需切换

#define amendment 1100

#define debug false

volatile long encoder_A = 0;
volatile long encoder_C = 0;

long temp_ena, temp_enc = 0;

long prev_A, prev_C = 0;
volatile long odometer = 0;
double velocity_A = 0.0;
double velocity_B = 0.0;
short pwm_A = 120;
short pwm_B = 120;

short pwm_F_L = 250;
short pwm_F_R = 250;
short pwm_B_L = 250;
short pwm_B_R = 250;

// flags here
volatile byte flag = 0;
bool start_flag = false;
bool pre_deceleration = false;
bool pass = false;

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
	pinMode(ENCODER_C, INPUT);
	pinMode(CRASH_BTN, INPUT);

	//  wifi.addAP("叁壹零", "sanyiling");
	//  while (wifi.run() != WL_CONNECTED)
	//  {
	//    delay(50);
	//  }
	//  server.on("/", handleRoot);
	//  server.on("/update_varible", handleupdate_varible);
	//  server.on("/update_speed", handleupdate_speed);
	//  server.on("/act", handleact);
	//  server.on("/modify", handlemodify);
	//  server.begin();
	//
	//  while (!start_flag)
	//  {
	//    server.handleClient();
	//  }

	const char *ssid = "ESP8266";
	const char *password = "12345678";

	WiFi.softAP(ssid, password);

	server.on("/", handleRoot);
	server.on("/update_varible", handleupdate_varible);
	server.on("/update_speed", handleupdate_speed);
	server.on("/act", handleact);
	server.on("/modify", handlemodify);
	server.begin();

	while (!start_flag)
	{
		server.handleClient();
	}

	// delay(5000);

	attachInterrupt(digitalPinToInterrupt(ENCODER_A), ISR_enc_A, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENCODER_C), ISR_enc_C, CHANGE);
	attachInterrupt(digitalPinToInterrupt(CRASH_BTN), crashDetect, RISING);

	digitalWrite(MOTOR_A_POS, HIGH);
	digitalWrite(MOTOR_A_NEG, LOW);
	digitalWrite(MOTOR_B_POS, HIGH);
	digitalWrite(MOTOR_B_NEG, LOW);

	startUpProcess();
}

void loop()
{
	server.handleClient();

	speedDetect();

	if (!pre_deceleration)
		crashPreProcess();

	long avg = ((encoder_A + encoder_C) / 2) - odometer + amendment;

	if (avg > odometer && odometer != 0)
	{
		digitalWrite(MOTOR_A_NEG, LOW);
		digitalWrite(MOTOR_B_NEG, LOW);
		flag = 2; //标识一切完成
	}
}

void handleRoot()
{
	String html = webpage;
	server.send(200, "text/html", html);
}

void handleupdate_varible()
{
	server.send(
		200,
		"text/json",
		"{ \"ena\":" + String(encoder_A) +
			", \"enc\":" + String(encoder_C) +
			", \"odometer\":" + String(odometer) +
			'}');
}

void handleact()
{
	String argument = server.arg("selector");
	if (argument == "0")
	{
		start_flag = true;
		server.send(200, "text/plain", "ok");
	}
	else if (argument == "1")
	{
		encoder_A = encoder_C = 0;
		odometer = 0;
		server.send(200, "text/plain", "ok");
	}
}

void handlemodify()
{
	// 未知大小可能出现问题

	if (server.hasArg("plain") == true)
	{
		StaticJsonDocument<200> doc;
		String json_text = server.arg("plain");
		DeserializationError error = deserializeJson(doc, json_text);
		pwm_F_L = doc["pwm_F_L"].as<short>();
		pwm_F_R = doc["pwm_F_R"].as<short>();
		pwm_B_L = doc["pwm_B_L"].as<short>();
		pwm_B_R = doc["pwm_B_R"].as<short>();

		if (!error)
		{
			pass = true;
			server.send(200, "text/plain", "ok");
		}
		else
		{
			server.send(500, "text/plain", "error");
		}
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
	if (curr - prev >= 40) // 检测时间阈值
	{
		velocity_A = (encoder_A - temp_ena);
		velocity_B = (encoder_C - temp_enc);

		if (!(pre_deceleration && flag == 0))
			speedAdjust();
		// 指定在预减速完成后的区间内不进行速度调整

		temp_ena = encoder_A;
		temp_enc = encoder_C;
		prev = millis();
	}
}

void speedAdjust()
{
	// PWM:A -> 右轮, PWM:B -> 左轮;
	if (flag == 0)
	{
		if (encoder_A < encoder_C)
		{
			if (debug)
			{
				pwm_A = pwm_F_L;
				pwm_B = pwm_F_R;
			}
			else
			{
				pwm_A = 131;
				pwm_B = 90;
			}
		}
		else
		{
			pwm_B = 110;
			pwm_A = 90;
		}
	}
	else if (flag == 1)
	{
		if (encoder_A < encoder_C)
		{
			if (debug)
			{
				pwm_A = pwm_B_L;
				pwm_B = pwm_B_R;
			}
			else
			{
				pwm_A = 114;
				pwm_B = 90;
			}
		}
		else
		{
			pwm_B = 116;
			pwm_A = 90;
		}
	}
	else
	{
		pwm_A = 0;
		pwm_B = 0;
	}
	analogWrite(PWMA, pwm_A);
	analogWrite(PWMB, pwm_B);
}

void crashPreProcess()
{
	// slow down the vehicle to prevent the vehicle from crashing

	if (encoder_A >= DECELERATION_THRESOLD || encoder_C >= DECELERATION_THRESOLD)
	{
		for (int i = 0; i < 9; i++)
		{
			int subtractor = 5 * i;
			analogWrite(PWMA, 120 - subtractor);
			analogWrite(PWMB, 120 - subtractor);
			delay(20);
		}
		pre_deceleration = true;
	}
}

void startUpProcess() //可能问题

{
	for (int i = 0; i < 20; i++)
	{
		int additor = 2;
		analogWrite(PWMA, 80 + additor);
		analogWrite(PWMB, 80 + additor);
		delay(1);
		// 看起来是延迟小一点好一些
	}

	analogWrite(PWMA, pwm_A);
	analogWrite(PWMB, pwm_B);

	curr = prev = millis();
}