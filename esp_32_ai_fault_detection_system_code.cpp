/*
========================================================
AI + Drone + ESP32 Smart Fault Detection System
ESP32 Sensor Monitoring Code

Name  : Satyam Mahadev Abdulpure
Branch: Electrical Engineering
========================================================
*/

#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>

// ---------------- WIFI ----------------
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- SENSOR PINS ----------------
#define TEMP_PIN 34
#define VOLT_PIN 35
#define VIB_PIN 32

// ---------------- ALERT PINS ----------------
#define LED_PIN 26
#define BUZZER_PIN 25

// ---------------- VARIABLES ----------------
float temperature = 0;
float voltage = 0;
int vibration = 0;

String alertStatus = "NORMAL";

WebServer server(80);

// ======================================================
// READ TEMPERATURE SENSOR
// ======================================================
float readTemperature()
{
  int value = analogRead(TEMP_PIN);

  // Example conversion
  float voltageValue = value * (3.3 / 4095.0);

  // LM35 conversion
  float tempC = voltageValue * 100;

  return tempC;
}

// ======================================================
// READ VOLTAGE SENSOR
// ======================================================
float readVoltage()
{
  int value = analogRead(VOLT_PIN);

  float sensorVoltage = value * (3.3 / 4095.0);

  // Adjust scaling factor according to sensor
  float actualVoltage = sensorVoltage * 10;

  return actualVoltage;
}

// ======================================================
// READ VIBRATION SENSOR
// ======================================================
int readVibration()
{
  int vib = analogRead(VIB_PIN);

  return vib;
}

// ======================================================
// ALERT CHECK
// ======================================================
void checkAlerts()
{
  if (temperature > 45 || voltage > 12 || vibration > 2500)
  {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    alertStatus = "HIGH RISK";
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    alertStatus = "NORMAL";
  }
}

// ======================================================
// WEB DASHBOARD PAGE
// ======================================================
void handleRoot()
{
  String page = "";

  page += "<html><head>";
  page += "<title>AI Fault Detection System</title>";

  page += "<style>";
  page += "body{font-family:Arial;background:#0f172a;color:white;text-align:center;}";
  page += ".card{background:#1e293b;padding:20px;margin:20px;border-radius:15px;}";
  page += "h1{color:#38bdf8;}";
  page += "</style>";

  page += "</head><body>";

  page += "<h1>AI + ESP32 Fault Detection Dashboard</h1>";

  page += "<div class='card'>";
  page += "<h2>Temperature</h2>";
  page += "<h3>" + String(temperature) + " °C</h3>";
  page += "</div>";

  page += "<div class='card'>";
  page += "<h2>Voltage</h2>";
  page += "<h3>" + String(voltage) + " V</h3>";
  page += "</div>";

  page += "<div class='card'>";
  page += "<h2>Vibration</h2>";
  page += "<h3>" + String(vibration) + "</h3>";
  page += "</div>";

  page += "<div class='card'>";
  page += "<h2>Alert Status</h2>";
  page += "<h2>" + alertStatus + "</h2>";
  page += "</div>";

  page += "</body></html>";

  server.send(200, "text/html", page);
}

// ======================================================
// SETUP
// ======================================================
void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  // WIFI CONNECT
  WiFi.begin(ssid, password);

  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  // WEB SERVER
  server.on("/", handleRoot);

  server.begin();

  lcd.setCursor(0, 1);
  lcd.print("System Ready");
}

// ======================================================
// LOOP
// ======================================================
void loop()
{
  // READ SENSORS
  temperature = readTemperature();
  voltage = readVoltage();
  vibration = readVibration();

  // CHECK ALERT
  checkAlerts();

  // SERIAL MONITOR
  Serial.println("------------");
  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Voltage: ");
  Serial.println(voltage);

  Serial.print("Vibration: ");
  Serial.println(vibration);

  Serial.print("Status: ");
  Serial.println(alertStatus);

  // LCD DISPLAY
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);

  lcd.print(" V:");
  lcd.print(voltage);

  lcd.setCursor(0, 1);
  lcd.print(alertStatus);

  // HANDLE WEB PAGE
  server.handleClient();

  delay(2000);
}
