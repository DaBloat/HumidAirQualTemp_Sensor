#include <MQ135.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define DHTTYPE DHT11

// Basic Setup
int dhtpin = 26; // DHT11 Output Pin (ESP32)
int mqpin = 34; // MQ135 Output Pin (ESP32)

DHT dht11(dhtpin, DHTTYPE); 
MQ135 mq135 = MQ135(mqpin);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

float temp_data = 0;
float hum_data = 0;
float air_data = 0;
unsigned long start;
unsigned long ave_start;
unsigned long curr;
const unsigned long interval = 60000;
const unsigned long ave_interval = 600000;

const char* ssid = "TIPQC-AUN"; // Wifi SSID
const char* password = "1962@tip"; // Wifi Password
String Web_App_URL = "https://script.google.com/macros/s/AKfycbyPa_lK3UVCOLnACHrQyveY6JjgmSkJORLfCrnxOrysfnade2MMuGy3lENh6kzrNqUL/exec"; //Web app in Google Scripts



void WifiSetUP(const char* ssid, const char* pass) {
  // To Setup your Wifi Connection using ESP32 and WIFI library
  WiFi.mode(WIFI_STA); // Sets the ESP 32 in Station mode
  WiFi.begin(ssid, pass); // Connects to the Wifi
  

  delay(1000);
  
  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;

  while (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Connecting to");
    lcd.setCursor(0,1);
    lcd.print(ssid);
    delay(500);
    lcd.clear(); // Waiting Effect
    delay(500);
    
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Connecting");
      lcd.setCursor(0,1);
      lcd.print("failed !");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ESP32 restarted");
      delay(1000);
      ESP.restart();
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi connected");
  lcd.setCursor(0,1);
  lcd.print("successfully.");
  delay(2000);
  lcd.clear();
}


void sendData(String params) {
   // Send the Data to the Google Sheets using the Web App URL
   HTTPClient http;
   String url=Web_App_URL+params;
   Serial.print(url);
   Serial.print("Making a request");
   http.begin(url.c_str()); //Specify the URL and certificate
   int httpCode = http.GET();
   http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
   http.end();
   Serial.println(": done "+httpCode);
}

void display_ppm() {
  // Display the CO2 (Carbon Dioxide) in Air by ppm
  int16_t raw = analogRead(mqpin); // Gets the Raw Data got form the Sensor
  Serial.print("raw: ");
  Serial.println(raw);
  
  float rzero = mq135.getRZero(); // RZero are useful for more accuracy
  Serial.print("Sensor Rzero: ");
  Serial.println(rzero);

  MQ135 new_mq135 = MQ135(mqpin,rzero); // Feedback loop
  float ppm = new_mq135.getPPM(); // more acceptable ppm
  Serial.print("C02 ppm: ");
  Serial.println(ppm);

  // LCD Printing
  lcd.setCursor(2,0);
  lcd.print("Air Quality: ");
  lcd.setCursor(3,1);
  lcd.print(ppm);
  lcd.print(" ppm");
}

float get_ppm() {
   float rzero = mq135.getRZero();
   MQ135 new_mq135 = MQ135(mqpin,rzero);
   float ppm = new_mq135.getPPM();
   return ppm;
}

void display_temperature() {
  // read temperature in Celsius
  float t = dht11.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(t);
  
  // LCD Printing
  lcd.setCursor(2,0);
  lcd.print("Temperature: ");
  lcd.setCursor(4,1);
  lcd.print(t);
  lcd.print(" C");
}

float get_temp() {
   float t = dht11.readTemperature();
   return t;
}

void display_humidity() {
  // read temperature in Celsius
  float h = dht11.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(h);

  // LCD Printing
  lcd.setCursor(3,0);
  lcd.print("Humidity : ");
  lcd.setCursor(5,1);
  lcd.print(h);
  lcd.print(" ");
}

float get_humid() {
   float h = dht11.readHumidity();
   return h;
}

void Dino() {
  byte dino1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B10000,
  B11000,
  B11111
};

byte dino2[8] = {
  B00000,
  B00001,
  B00001,
  B00001,
  B00001,
  B00011,
  B01111,
  B11111
};
byte dino3[8] = {
  B11111,
  B10111,
  B11111,
  B11100,
  B11111,
  B11100,
  B11100,
  B11100
};
byte dino4[8] = {
  B10000,
  B11000,
  B11000,
  B00000,
  B10000,
  B00000,
  B00000,
  B00000
};
byte dino5[8] = {
  B11111,
  B11111,
  B01111,
  B00111,
  B00011,
  B00011,
  B00010,
  B00011
};
byte dino6[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B10110,
  B00010,
  B00010,
  B00011
};

byte dino7[8] = {
  B11111,
  B11001,
  B10000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

  lcd.createChar(1,dino1);
  lcd.createChar(2,dino2);
  lcd.createChar(3,dino3);
  lcd.createChar(4,dino4);
  lcd.createChar(5,dino5);
  lcd.createChar(6,dino6);
  lcd.createChar(7,dino7);
  for (int a=0; a<=18; a++)
  {


  if(a>=3){
  lcd.setCursor(a-3,0);
  lcd.write(1);}
  if(a>=2)
  {lcd.setCursor(a-2,0);
  lcd.write(2);}
  if(a>=1)
  {lcd.setCursor(a-1,0);
  lcd.write(3);}
  lcd.setCursor(a,0);
  lcd.write(4);
  
  if(a>=3){
  lcd.setCursor(a-3,1);
  lcd.write(5);}
  if(a>=2){
  lcd.setCursor(a-2,1);
  lcd.write(6);}
  if(a>=1){
  lcd.setCursor(a-1,1);
  lcd.write(7);}
  
  delay(400);
  lcd.clear();
 }
}

  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Baud Rate is higher in ESP32

  // LCD Setup
  lcd.init();
  lcd.backlight();

  // DHT 11 Setup
  dht11.begin();

  // Wifi Setup
  WifiSetUP(ssid,password);

  // MOTD
  lcd.setCursor(3,0);
  lcd.print("Welcome To");
  lcd.setCursor(0,1);
  lcd.print("Beyonce's Sensor");
  delay(5000);
  lcd.clear();
  delay(2000);

  start = millis();
}

void loop() {  
  // display loop
  // display temperature
  display_temperature();
  delay(2000);
  lcd.clear();
  
  // display humidity
  display_humidity();
  delay(2000);
  lcd.clear();

  // display air quality
  display_ppm();
  delay(2000);
  lcd.clear();

  // recording data
  curr = millis();
  if (curr - start >= interval) {
    Serial.println("1 minute See you Tommorow");
    temp_data += get_temp();
    hum_data += get_humid();
    air_data += get_ppm();
    start = curr;
  }

  if (curr - ave_start >= ave_interval) {
    Serial.println("10 minute napo!");
    temp_data /= 10;
    hum_data /= 10;
    air_data /= 10;

    String temp(temp_data);
    String humid(hum_data);
    String ppm(air_data);
    String params = "?sts=write&temp="+temp+"&humd="+humid+"&quality="+ppm;
    sendData(params);

    temp_data = 0;
    hum_data = 0;
    air_data = 0;
    
    ave_start = curr;
  }

  Dino();
  Serial.println("-------------");
  delay(1000);
}
