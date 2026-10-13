#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C   // try 0x3D if the display isn't detected
#define OLED_RESET -1    // no reset pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);
    delay(500);  // let the serial monitor connect

    Wire.begin(OLED_SDA, OLED_SCL);
    Wire.setClock(400000);  // 400 kHz fast-mode I2C

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED initialization failed! Check wiring and I2C address.");
        while (true) {
            delay(100);
        }
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Hello IoT");
    display.println("Class Fellows!");
    display.println();
    display.println("ESP32 + OLED Ready");

    display.display();

    Serial.println("OLED initialized successfully.");
}

void loop() {
    // Add your application code here.
}