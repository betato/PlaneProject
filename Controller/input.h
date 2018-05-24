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

#define PITCH_MAX 100
#define PITCH_MIN 20
int in_pitchConv(int pitch) {
	float lerpVal = (float)pitch / INPUT_MAX;
	return lerp(PITCH_MIN, PITCH_MAX, lerpVal);
}

#define ROLL_MAX 100
#define ROLL_MIN 20
int in_rollConv(int roll) {
	float lerpVal = (float)roll / INPUT_MAX;
	return lerp(ROLL_MIN, ROLL_MAX, lerpVal);
}

#define YAW_MAX 100
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

int in_pitch = 0;
int in_roll = 0;
int in_yaw = 0;
int in_throttle = 0;

bool in_pitchChg = false;
bool in_rollChg = false;
bool in_yawChg = false;
bool in_throttleChg = false;

void in_init() {
	pinMode(PIN_PITCH, INPUT);
	pinMode(PIN_ROLL, INPUT);
	pinMode(PIN_YAW, INPUT);
	pinMode(PIN_THROTTLE, INPUT);
}

void in_read_input(uint8_t pin, int* value, bool* chg) {
	int newValue = analogRead(pin);
	*chg = (abs(*value - newValue) > INPUT_EPSILON);
	*value = newValue;
}

void in_update() {
	in_read_input(PIN_PITCH, &in_pitch, &in_pitchChg);
	in_read_input(PIN_ROLL, &in_roll, &in_rollChg);
	in_read_input(PIN_YAW, &in_yaw, &in_yawChg);
	in_read_input(PIN_THROTTLE, &in_throttle, &in_throttleChg);
}