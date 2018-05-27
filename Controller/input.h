#pragma once

#include <common_packet.h>

#define PIN_PITCH A0
#define PIN_ROLL A1
#define PIN_YAW A3
#define PIN_THROTTLE A6	

#define INPUT_MAX 1023
#define JOY_MIN 0

//the minimum change in input that we'll send. this accounts for analog noise
#define INPUT_EPSILON 4

int lerp(int a, int b, float f) {
	return a + (int)(f * (b - a));
}

#define PITCH_MAX 160
#define PITCH_MIN 20
int in_pitchConv(int pitch) {
	float lerpVal = (float)pitch / INPUT_MAX;
	return lerp(PITCH_MIN, PITCH_MAX, lerpVal);
}

#define ROLL_MAX 160
#define ROLL_MIN 20
int in_rollConv(int roll) {
	float lerpVal = (float)roll / INPUT_MAX;
	return lerp(ROLL_MIN, ROLL_MAX, lerpVal);
}

#define YAW_MAX 160
#define YAW_MIN 20
int in_yawConv(int yaw) {
	float lerpVal = (float)yaw / INPUT_MAX;
	return lerp(YAW_MIN, YAW_MAX, lerpVal);
}

#define THROTTLE_MAX 255
#define THROTTLE_MIN 0
int in_throttleConv(int throttle) {
	float lerpVal = (float)throttle / INPUT_MAX;
	return lerp(THROTTLE_MIN, THROTTLE_MAX, lerpVal);
}

uint8_t in_pitch = 0;
uint8_t in_roll = 0;
uint8_t in_yaw = 0;
uint8_t in_throttle = 0;

#define IN_UPDATE_INTERVAL 10
unsigned long in_lastUpdate;

void in_init() {
	pinMode(PIN_PITCH, INPUT);
	pinMode(PIN_ROLL, INPUT);
	pinMode(PIN_YAW, INPUT);
	pinMode(PIN_THROTTLE, INPUT);
	in_lastUpdate = millis();
}


void in_update() {
	if (millis() - in_lastUpdate > IN_UPDATE_INTERVAL) {
		ControlInput input;
		input.data.pitch = in_pitchConv(in_pitch);
		input.data.roll = in_rollConv(in_roll);
		input.data.yaw = in_yawConv(in_yaw);
		input.data.throttle = in_throttleConv(in_throttle);
		pkt_sendControlInput(&input);
		in_lastUpdate = millis();
	}
}