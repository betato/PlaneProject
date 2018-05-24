#include <Servo.h>
//#include <common_packet.h>

#define HEARTBEAT_INTERVAL 2000

#define PIN_ROLL 3
#define PIN_PITCH 5
#define PIN_YAW 6

//this is actually two servos
Servo rollServo;
Servo pitchServo;
Servo yawServo;

unsigned long lastTransmit;

void setup() {
	Serial.begin(9600);
	pinMode(7, OUTPUT);
	digitalWrite(7, HIGH);
	//pkt_initRadio();
	pinMode(PIN_ROLL, OUTPUT);
	pinMode(PIN_PITCH, OUTPUT);
	pinMode(PIN_YAW, OUTPUT);
	rollServo.attach(PIN_ROLL, 1000, 2000);
	pitchServo.attach(PIN_PITCH, 1000, 2000);
	yawServo.attach(PIN_YAW, 1000, 2000);
}

void loop() {
	//pkt_update();
	rollServo.write(0);
	pitchServo.write(180);
	yawServo.write(180);
}