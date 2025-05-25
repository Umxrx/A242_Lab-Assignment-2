#include <WiFi.h>
#include <FirebaseESP32.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Replace with your network credentials
#define WIFI_SSID "myUUM_Guest"
#define WIFI_PASSWORD ""

// Replace with your Firebase project credentials
#define API_KEY "MY_API_KEY"
#define DATABASE_URL "https://my-project-id-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "email@example.com"
#define USER_PASSWORD "my_password"

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// EEPROM addresses
#define EEPROM_SIZE 512
#define SSID_ADDR 0
#define PASS_ADDR 100

String latestMessage = "";

void writeStringToEEPROM(int addrOffset, const String &strToWrite) {
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  EEPROM.commit();
}

String readStringFromEEPROM(int addrOffset) {
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';
  return String(data);
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Store Wi-Fi credentials into EEPROM // Comment it out later
  writeStringToEEPROM(SSID_ADDR, WIFI_SSID);
  writeStringToEEPROM(PASS_ADDR, WIFI_PASSWORD);

  // Retrieve stored Wi-Fi credentials
  String ssid = readStringFromEEPROM(SSID_ADDR);
  String password = readStringFromEEPROM(PASS_ADDR);

  WiFi.begin(ssid.c_str(), password.c_str());
  
  display.setCursor(0, 0);
  display.println("Connecting to Wi-Fi...");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Wi-Fi connected");
  display.display();

  // Configure Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  if (!Firebase.beginStream(fbdo, "/oled_text")) {
    Serial.println("Could not begin stream: " + fbdo.errorReason());
  }
  Firebase.reconnectWiFi(true);
}

void scrollMessage(String message) {
  int textWidth = message.length() * 6; // approx width per char
  static int scrollX = SCREEN_WIDTH;
  static unsigned long lastScroll = 0;

  if (millis() - lastScroll > 50) {
    display.clearDisplay();
    display.setCursor(scrollX, 0);
    display.print(message);
    display.display();
    
    scrollX--;
    if (scrollX < -textWidth) {
      scrollX = SCREEN_WIDTH; // restart scroll
    }

    lastScroll = millis();
  }
}

void loop() {
  if (!Firebase.readStream(fbdo)) {
    Serial.println("Stream read failed: " + fbdo.errorReason());
    return;
  }

  if (fbdo.streamTimeout()) {
    Serial.println("Stream timeout, reconnecting...");
    return;
  }

  if (fbdo.streamAvailable()) {
    if (fbdo.dataType() == "string") {
      latestMessage = fbdo.stringData();
      Serial.println("Streamed: " + latestMessage);
    }
  }

  scrollMessage(latestMessage); // Keep scrolling the latest message
}
