#include <ServoTimer2.h>
#include <common_packet.h>

#define HEARTBEAT_INTERVAL 2000

#define PIN_ROLL 3
#define PIN_PITCH 5
#define PIN_YAW 6

//the roll servo is actually two servos attached together
ServoTimer2 rollServo;
ServoTimer2 pitchServo;
ServoTimer2 yawServo;

unsigned long lastTransmit;

void setup() {
	Serial.begin(9600);
	pinMode(7, OUTPUT);
	digitalWrite(7, HIGH);
	pkt_initRadio();
	pinMode(PIN_ROLL, OUTPUT);
	pinMode(PIN_PITCH, OUTPUT);
	pinMode(PIN_YAW, OUTPUT);
	rollServo.attach(PIN_ROLL);
	pitchServo.attach(PIN_PITCH);
	yawServo.attach(PIN_YAW);
}

int angleToMicros(int angle) {
	float lerpFac = (float)angle / (float)180;
	return 1000 + (int)(lerpFac * (float)(2000 - 1000));
}

void loop() {
	pkt_update();
	if (pkt_available()) {
		if (pkt_payloadType == CONTROL_INPUT) {
			ControlInput input = pkt_readControlInput();
			pitchServo.write(angleToMicros(input.data.pitch));
			rollServo.write(angleToMicros(input.data.pitch));
			yawServo.write(angleToMicros(input.data.yaw));
		}
		else {
			Serial.println(pkt_payloadType);
			pkt_readPacket();
		}
	}
}