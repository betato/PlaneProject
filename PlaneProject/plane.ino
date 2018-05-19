#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define OLED_RESET
Adafruit_SSD1306 lcd(4);

#define RADIO_TX 3
#define RADIO_RX 2
SoftwareSerial radio(RADIO_RX, RADIO_TX);

void setup() {
	//init the lcd screen
	lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	lcd.clearDisplay();
	lcd.setTextSize(1);
	lcd.setTextColor(WHITE);
	//init the radio
	radio.begin(9600);
}

void loop() {
	if (radio.available()) {
		int response = radio.read();
		if (response == 6) {
			lcd.clearDisplay();
			lcd.setCursor(0, 0);
			lcd.println("HEARTBEAT");
			lcd.println(millis());
		}
	}
}