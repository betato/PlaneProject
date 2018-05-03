#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <MPU9250_asukiaaa.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

Adafruit_BMP280 bmp;
MPU9250 mpu;

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

void initGyro() {
	Wire.begin();
	mpu.setWire(&Wire);
	mpu.beginAccel();
	mpu.beginGyro();
	mpu.beginMag();
}

void setup() {
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);
	initLcd();
	initBmp();
	initGyro();
}

float mpuData[10];

void displayData() {
	display.clearDisplay();
	mpu.accelUpdate();
	mpu.gyroUpdate();
	mpu.magUpdate();
	display.setCursor(0, 0);
	display.print("A:(");
	display.print(mpu.accelX());
	display.print(",");
	display.print(mpu.accelY());
	display.print(",");
	display.print(mpu.accelZ());
	display.println(")");
	display.print("G:(");
	display.print(mpu.gyroX());
	display.print(",");
	display.print(mpu.gyroY());
	display.print(",");
	display.print(mpu.gyroZ());
	display.println(")");
	display.print("M:(");
	display.print(mpu.magX());
	display.print(",");
	display.print(mpu.magY());
	display.print(",");
	display.print(mpu.magZ());
	display.println(")");
	display.print("Temp: ");
	display.println(bmp.readTemperature());
	display.print("Altitude: ");
	display.println(bmp.readAltitude());
	display.display();
}

void loop() {
	displayData();
	delay(200);
}