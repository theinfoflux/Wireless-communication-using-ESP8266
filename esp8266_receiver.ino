
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 
// Must match the sender structure
int led=D5;

typedef struct struct_message {
  int potvalue;
  int buttonstatus;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Button status: ");
  Serial.println(myData.buttonstatus);
  Serial.print("Potvalue: ");
  Serial.println(myData.potvalue);
  Serial.println();

lcd.setCursor(0, 0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("PotValue: "); 
  lcd.print(myData.potvalue);
  delay(10);
 
  if(myData.buttonstatus==LOW)
  {
    digitalWrite(led,HIGH);
    }
    else
    {
      digitalWrite(led,LOW);
      }
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
 lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on 
  pinMode(led,OUTPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);




  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop() {
  
}
