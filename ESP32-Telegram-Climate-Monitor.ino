#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
 
// ---
CONFIGURATION ---
const char* ssid     = "YOUR PASSWORD NAME HERE"; 
const char* password = "PASSWORD HERE";
#define BOT_TOKEN    "YOUR TELEGRAM BOT TOKEN HERE" 
#define CHAT_ID      "YOUR TELEGRAM ID HERE"
 
// ---
HARDWARE ---
#define DHTPIN       4
#define DHTTYPE    
 DHT11
#define LED_WHITE    17
#define LED_BLUE     16
#define LED_RED      27
#define BUZZER       26
 
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306display(128, 64, &Wire, -1);
WiFiClientSecure
client;
UniversalTelegramBotbot(BOT_TOKEN, client);
 
// ---
GLOBALS ---
String
pendingMessage = "";
bool botNeedsReauth = true; 
unsigned long lastUpdate = 0;
unsigned long lastWiFiCheck = 0;
unsigned long lastBotCheck = 0;
int lastState = 1; // Start in STABLE state
(1) to prevent false boots
 
//
Buzzer management
unsigned long buzzerStartTime = 0;
bool isBuzzerRinging = false;
const unsigned long buzzerDuration = 1500; // Ring for exactly 1.5
seconds
 
float currentTemp = 0.0;
float currentHum = 0.0;
String
currentStatus = "STARTING...";
 
void setup() {
  Serial.begin(115200);
  pinMode(LED_WHITE, OUTPUT); pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT); pinMode(BUZZER, OUTPUT);
 
  dht.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for(;;);
  display.setRotation(2);
  display.clearDisplay();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}
 
void loop() {
  unsigned long currentMillis = millis();
 
 
// Non-blocking single-beep management timer
  if (isBuzzerRinging
&& (currentMillis - buzzerStartTime >= buzzerDuration)) {
 
  digitalWrite(BUZZER, LOW);
 
  isBuzzerRinging = false;
  }
 
 
// 1. NON-BLOCKING WIFI & BOT MANAGEMENT
  if (WiFi.status() != WL_CONNECTED) {
 
  if (currentMillis -
lastWiFiCheck > 10000) {
 
    WiFi.begin(ssid, password);
 
    lastWiFiCheck = currentMillis;
 
    botNeedsReauth = true; 
 
  }
  } else {
 
  if (botNeedsReauth) {
 
    client.setInsecure();
 
    botNeedsReauth = false;
 
    bot.sendMessage(CHAT_ID, "✅ System Online/Reconnected!\nSend /status anytime.", "");
 
  }
 
  
 
  if (pendingMessage != "") {
 
    if (bot.sendMessage(CHAT_ID, pendingMessage, "")) {
 
      pendingMessage = "";
 
    }
 
  }
 
 
  // 2. REMOTE TELEGRAM COMMAND CHECK (Runs every 5 seconds
asynchronously)
 
  if (currentMillis -
lastBotCheck >= 5000) {
 
    lastBotCheck = currentMillis;
 
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    
 
    while (numNewMessages) {
 
      for (int i = 0; i < numNewMessages; i++) {
 
        String chat_id = String(bot.messages[i].chat_id);
 
        if (chat_id != CHAT_ID) continue; 
 
        
 
        String text = bot.messages[i].text;
 
        if (text == "/status") {
 
          String msg = "🌡 Current Temp: " + String((int)currentTemp) + "°C\n";
 
          msg += "💧 Humidity: " + String((int)currentHum) + "%\n";
 
          msg += "📊 Climate Status: " + currentStatus;
 
          bot.sendMessage(CHAT_ID, msg, "");
 
        }
 
      }
 
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    }
 
  }
  }
 
 
// 3. SENSOR & DISPLAY LOGIC (Runs every 2s, independent of WiFi)
  if (currentMillis -
lastUpdate >= 2000) {
 
  lastUpdate = currentMillis;
 
  float temp = dht.readTemperature();
 
  float hum = dht.readHumidity();
 
 
  if (!isnan(temp) && !isnan(hum)) {
 
    currentTemp = temp;
 
    currentHum = hum;
 
 
    display.clearDisplay();
 
    display.setTextColor(SSD1306_WHITE);
 
    
 
    
 
    // Print temperature with the degree symbol and C
 
    display.setTextSize(3);
 
    display.setCursor(0,0);
 
    display.print((int)temp); 
 
    display.write(247); 
 
    display.print("C");
 
    
 
    // Print humidity
 
    display.setTextSize(1);
 
    display.setCursor(90,30);
 
    display.print("H:");
 
    display.print((int)hum); 
 
    display.print("%");
 
    
 
    // Smart Hysteresis State Buffer Logic
 
    int currentState = lastState; // Default stay in current state
 
    
 
    if (lastState == 1) { // Currently STABLE
 
      if (temp < 24.0) currentState = 0;      // Trigger COLD
 
      else if (temp > 33.0) currentState = 2; // Trigger HOT
 
    } 
 
    else if (lastState == 0) { // Currently COLD
 
      if (temp >= 25.0) currentState = 1;     // Warm enough to clear back to STABLE
 
    } 
 
    else if (lastState == 2) { // Currently HOT
 
      if (temp <= 32.0) currentState = 1;     // Cool enough to clear back to STABLE
 
    }
 
 
    display.setTextSize(1);
 
    display.setCursor(0, 45);
 
    
 
    if (currentState == 0) {
 
      currentStatus = "TOO COLD";
 
      display.print("STATUS: TOO COLD");
 
      digitalWrite(LED_BLUE, HIGH); digitalWrite(LED_RED, LOW); digitalWrite(LED_WHITE, LOW);
 
    } else if (currentState == 2) {
 
      currentStatus = "TOO HOT";
 
      display.print("STATUS: TOO HOT");
 
      digitalWrite(LED_RED, HIGH); digitalWrite(LED_BLUE, LOW); digitalWrite(LED_WHITE, LOW);
 
    } else {
 
      currentStatus = "STABLE";
 
      display.print("STATUS: STABLE");
 
      digitalWrite(LED_WHITE, HIGH); digitalWrite(LED_BLUE, LOW); digitalWrite(LED_RED, LOW);
 
    }
 
    
 
    // Corrected Display text: "Wi-Fi Connected" instead of
"Connected WiFi"
 
    display.setCursor(0, 56);
 
    if (WiFi.status() == WL_CONNECTED) {
 
      display.print("[ Wi-Fi Connected ]");
 
    } else {
 
      display.print("[ Offline Mode ]");
 
    }
 
    
 
    display.display();
 
 
    // Trigger automatic alert and single beep on actual change
 
    if (currentState != lastState) {
 
      String msg = (currentState == 0) ? "🔵 Alert: TOO COLD" : (currentState == 2) ? "🔴 Alert: TOO HOT" : "🟢 Alert: Back to
STABLE";
 
      pendingMessage = msg + "\nTemp: " + String((int)temp) + "°C\nHumidity:
" + String((int)hum) + "%";
 
      
 
      // Single beep logic execution
 
      digitalWrite(BUZZER, HIGH);
 
      buzzerStartTime = millis();
 
      isBuzzerRinging = true;
 
 
      lastState = currentState;
 
    }
 
  }
  }
}
