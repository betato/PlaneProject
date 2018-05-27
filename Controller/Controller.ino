#include <common_packet.h>
#include "input.h"

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 lcd(6);

void initLcd() {
	//set up the lcd screen
	lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	lcd.clearDisplay();
	lcd.setTextSize(1);
	lcd.setTextColor(WHITE);
}

void setup() {
	Serial.begin(9600);
	pkt_initRadio();
	//initLcd();
	//set the radio transmitter to normal mode
	pinMode(7, OUTPUT);
	digitalWrite(7, HIGH);
	in_init();
}

char buf[20];

void displayInputs() {
	lcd.clearDisplay();
	lcd.setCursor(0, 0);
	sprintf(buf, "Pitch: %d", in_pitchConv(in_pitch));
	lcd.println(buf);
	sprintf(buf, "Roll: %d", in_rollConv(in_roll));
	lcd.println(buf);
	sprintf(buf, "Yaw: %d", in_yawConv(in_yaw));
	lcd.println(buf);
	sprintf(buf, "Throttle: %d", in_throttleConv(in_throttle));
	lcd.println(buf);
	lcd.println(millis());
	lcd.display();
}

void loop() {
	in_update();
}