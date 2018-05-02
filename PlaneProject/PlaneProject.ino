#define SERVO_PWM_PERIOD 20 * 1000

enum ServoState {
	CYCLE_HIGH,
	CYCLE_LOW
};

struct ServoCtrl {
	uint8_t pin;
	int position;
	ServoState state;

	int lastCycleTime;
} testServo;

int srv_get_duty_cycle(int position) {
	//linear interpolate between 1 ms to 2 ms duty cycle
	int minAngle = -90;
	int maxAngle = 90;
	//all times are in microseconds
	int minCycle = 1000;
	int maxCycle = 2000;
	float lerp = (float)(position - minAngle) / (float)(maxAngle - minAngle);
	return minCycle + (lerp * (maxCycle - minCycle));
}

void srv_init(struct ServoCtrl* servo, uint8_t pin) {
	servo->pin = pin;
	pinMode(pin, OUTPUT);
	servo->position = 0;
	servo->lastCycleTime = micros();
	//initialize it to the high portion of the pwm cycle
	servo->state = CYCLE_HIGH;
	digitalWrite(servo->pin, HIGH);
}

void srv_update(struct ServoCtrl* servo) {
	unsigned long time = micros();
	if (servo->state == CYCLE_HIGH) {
		//check if its time to turn off the power
		if (time - servo->lastCycleTime > srv_get_duty_cycle(servo->position)) {
			digitalWrite(servo->pin, LOW);
			servo->state = CYCLE_LOW;
		}
	}
	if (servo->state == CYCLE_LOW) {
		//check if its time to start a new cycle
		if (time - servo->lastCycleTime > SERVO_PWM_PERIOD) {
			servo->lastCycleTime = micros();
			digitalWrite(servo->pin, HIGH);
			servo->state = CYCLE_HIGH;
		}
	}
}

void setup() {
	srv_init(&testServo, 6);
}

void loop() {
	srv_update(&testServo);
	testServo.position = 0;
}
