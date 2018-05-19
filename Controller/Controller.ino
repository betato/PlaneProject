#include <common_packet.h>

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
	Serial.println("setup");
	pkt_initRadio();
	initLcd();
	pinMode(7, OUTPUT);
	digitalWrite(7, HIGH);
	Serial.println("setup2");

}

void loop() {
	pkt_update();
	if (pkt_available) {
		switch (pkt_payloadType) {
		case JOY_INPUT:
			union JoyInput input = pkt_readJoyInput();
			lcd.setCursor(0, 0);
			lcd.clearDisplay();
			lcd.println(input.data.type);
			lcd.println(input.data.value);
			lcd.display();
			break;
		}
	}
}