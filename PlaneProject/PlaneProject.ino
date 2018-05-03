#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

Adafruit_BMP280 bmp;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define LED_ADDR = 0x3C

void initLcd() {
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
}

void initBmp() {
	if (!bmp.begin(0x76)) {
		digitalWrite(LED_BUILTIN, HIGH);
	}
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	initLcd();
	initBmp();
}

void loop() {
	display.clearDisplay();
	display.setCursor(0, 0);
	display.print((double)bmp.readTemperature());
	display.display();
	delay(50);
}