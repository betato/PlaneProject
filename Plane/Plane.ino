#include <common_packet.h>

#define HEARTBEAT_INTERVAL 2000

unsigned long lastTransmit;

void setup() {
	Serial.begin(9600);
	pinMode(7, OUTPUT);
	digitalWrite(7, HIGH);
	pkt_initRadio();
	lastTransmit = millis();
}

void loop() {
	union JoyInput joy;
	joy.data.type = CLICK;
	joy.data.value = millis();
	if (millis() - lastTransmit > HEARTBEAT_INTERVAL) {
		pkt_sendJoyInput(&joy);
		lastTransmit = millis();
	}
}