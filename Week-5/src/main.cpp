#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

#define DHT_PIN 19
#define DHT_TYPE DHT11

Adafruit_SSD1306 display(
    SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

DHT dht(DHT_PIN, DHT_TYPE);

// Maximum values used for bar scaling
#define MAX_TEMP 50.0
#define MAX_HUMIDITY 100.0

void setup()
{
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    Serial.println("OLED initialization failed!");

    while (true)
    {
      delay(100);
    }
  }

  // Initialize DHT11
  dht.begin();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(15, 20);
  display.println("ENVIRONMENT");
  display.setCursor(25, 35);
  display.println("MONITOR");

  display.display();

  delay(2000);
}

void loop()
{
  // Read sensor values
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check for sensor reading errors
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("DHT11 read failed!");

    display.clearDisplay();
    display.setCursor(10, 20);
    display.println("Sensor Error");
    display.setCursor(10, 35);
    display.println("Check DHT11");

    display.display();

    delay(2000);
    return;
  }

  // Print readings to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.print(" C    Humidity: ");
  Serial.print(humidity, 1);
  Serial.println(" %");

  // Calculate bar heights
  int tempHeight = (temperature / MAX_TEMP) * 30;
  int humidityHeight = (humidity / MAX_HUMIDITY) * 30;

  // Keep bar heights within display limits
  tempHeight = constrain(tempHeight, 0, 30);
  humidityHeight = constrain(humidityHeight, 0, 30);

  // Clear previous screen
  display.clearDisplay();

  // -------------------------------
  // TEMPERATURE VALUE
  // -------------------------------

  display.setTextSize(1);
  display.setCursor(20, 2);

  display.print(temperature, 1);
  display.print("C");

  // -------------------------------
  // HUMIDITY VALUE
  // -------------------------------

  display.setCursor(83, 2);

  display.print(humidity, 1);
  display.print("%");

  // -------------------------------
  // GRAPH GRID LINES
  // -------------------------------

  display.drawLine(5, 14, 122, 14, SSD1306_WHITE);
  display.drawLine(5, 25, 122, 25, SSD1306_WHITE);
  display.drawLine(5, 36, 122, 36, SSD1306_WHITE);
  display.drawLine(5, 48, 122, 48, SSD1306_WHITE);

  // -------------------------------
  // TEMPERATURE BAR
  // -------------------------------

  int tempX = 28;
  int barWidth = 20;
  int baseline = 48;

  display.fillRect(
      tempX,
      baseline - tempHeight,
      barWidth,
      tempHeight,
      SSD1306_WHITE);

  // -------------------------------
  // HUMIDITY BAR
  // -------------------------------

  int humidityX = 88;

  display.fillRect(
      humidityX,
      baseline - humidityHeight,
      barWidth,
      humidityHeight,
      SSD1306_WHITE);

  // -------------------------------
  // BAR LABELS
  // -------------------------------

  display.setCursor(24, 53);
  display.print("TEMP");

  display.setCursor(82, 53);
  display.print("HUMID");

  // Update physical OLED
  display.display();

  // Read sensor every 2 seconds
  delay(2000);
}