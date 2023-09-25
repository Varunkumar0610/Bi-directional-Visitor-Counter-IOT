/*************************************************************
  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - Arduino Uno board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "Blynk_twemp_id"
#define BLYNK_TEMPLATE_NAME "Project_name"
#define BLYNK_AUTH_TOKEN "Auth_token"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <LiquidCrystal.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "XXXXX";     // Replace "YourWiFiSSID" with your WiFi SSID
char pass[] = "XXXXX"; // Replace "YourWiFiPassword" with your WiFi password

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(7, 6); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define in  8
#define out 13
#define bulb 10
int count = 0;
const int ct = 9; //initialize PWM(9) as AnalogWrite to control brightness of lcd 

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  lcd.begin(16, 2);
  lcd.print("Bi-Directional");
  lcd.setCursor(0, 1);
  lcd.print(" Visitor Counter");
  delay(2000);

  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(bulb, OUTPUT);

  analogWrite(ct, 110);
  lcd.clear();
  lcd.print("Person In Room:");
  lcd.setCursor(0, 1);
  lcd.print(count);

  //Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
  // You can also specify server:
  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);
  Blynk.virtualWrite(V3, count);
  Blynk.virtualWrite(V4,in);
  Blynk.virtualWrite(V5,out);
  Blynk.virtualWrite(V6,bulb);
  // Blynk.virtualWrite(V0, LOW); // Set initial state for the virtual pin controlling inSensor
  // Blynk.virtualWrite(V1, LOW); // Set initial state for the virtual pin controlling outSensor
  // Blynk.virtualWrite(V2, LOW); // Set initial state for the virtual pin controlling the bulb
}
// 
void loop()
{
   int in_value = digitalRead(in);
  int out_value = digitalRead(out);

  if (in_value == LOW)
  {
    count++;
    lcd.clear();
    lcd.print("Person In Room:");
    Blynk.virtualWrite(V4, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(count);
    delay(1000);
    Blynk.virtualWrite(V3, count); 
    Blynk.virtualWrite(V4, LOW);               
  }

  if (out_value == LOW)
  {
    count--;
    lcd.clear();
    lcd.print("Person in Room");
    Blynk.virtualWrite(V5, HIGH);
    lcd.setCursor(0, 1);
    lcd.print(count);
    delay(1000);
    Blynk.virtualWrite(V3, count);
    Blynk.virtualWrite(V5, LOW);                   // Debounce delay
  }

  if (count > 0)
  {
    lcd.clear();
    digitalWrite(bulb, HIGH);
    lcd.clear();
    lcd.print("Person Entered");
    lcd.setCursor(0, 1);
    lcd.print("Light's ON :");
    Blynk.virtualWrite(V6, HIGH);
    lcd.print(count);
    delay(100);
  }
  else
  {
    lcd.clear();
    digitalWrite(bulb, LOW);
    lcd.clear();
    lcd.print("Nobody in Room");
    lcd.setCursor(0, 1);
    lcd.print("Light's OFF");
    Blynk.virtualWrite(V6, LOW);
    delay(200);
  }
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
