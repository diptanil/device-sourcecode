#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

const int MOISTURE_SENSOR = A0;

const char* ssid     = "Your WiFi SSID";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Your Wifi Password";     // The password of the Wi-Fi network

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int timerCounter = 0;

const int SensorDryVal = 690;
const int SensorWaterVal = 280;
int soilMoistureReading = 0;
int soilMoisturePercent = 0;

bool b_WifiConnected = true;

void setup()
{
  Serial.begin(115200);
  
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer.
  display.clearDisplay();

  // Display Initialization Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,12);
  display.println("Initializing!");
  display.display();
  delay(2000);

  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i <= 10) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

  // Clear the buffer.
  display.clearDisplay();
}

void loop() {
  soilMoistureReading = analogRead(MOISTURE_SENSOR);
  soilMoisturePercent = map(soilMoistureReading, SensorDryVal, SensorWaterVal, 0, 100);

  // WiFi Connectivity
  if (timerCounter <= 0){
    if (WiFi.status() != WL_CONNECTED){
      b_WifiConnected = false;
    }
    else{
      b_WifiConnected = true;
    }
    timerCounter = 5;
  }

  if (b_WifiConnected){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.fillCircle(5, 5, 5, WHITE);
    display.setCursor(15,2);
    display.println(WiFi.localIP());
    display.display();
  } else {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.drawCircle(5, 5, 5, WHITE);
    display.setCursor(15,2);
    display.println("DISCONNECTED!!!");
    display.display();
  }
  
  

  // Server Connectivity
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,12);
  display.println("SERVER 404");

  // Display Soil Mooisture
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,24);
  display.println("Mois % - ");
  display.setCursor(60,24);
  display.println(soilMoisturePercent);
  display.println("%");
  display.display();
  delay(2000);

  //Increment timer Counter
  timerCounter = timerCounter - 1;

  // Clear the buffer.
  display.clearDisplay();
}
