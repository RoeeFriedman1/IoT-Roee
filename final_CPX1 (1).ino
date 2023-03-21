/*
  CPX1
      This sketch represents the CPX1 located in the night eater bedroom. 
      It communicates with CPX2 through Blynk and sends messages accordingly.

  Created By:
  Roee Friedman 208743583
  Ido Bosmi 209794395
*/ 


#include <Adafruit_CircuitPlayground.h>

#define BLYNK_TEMPLATE_ID "XXXXXXXXXXX" // Insert your BLYNK_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME "XXX" // Insert your BLYNK_TEMPLATE_NAME
#define BLYNK_AUTH_TOKEN "XXXXXXXXXXXXXXXXXXXXXXXXXXXX" // Insert your BLYNK_AUTH_TOKEN

//Defines for a Samsung TV using NECx protocol, change according to your TV.
#define MY_PROTOCOL NECX
#define MY_BITS 32
#define MY_POWER 0xE0E040BF


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
char ssid[] = "XXX"; // Set to your netwrok name.
char pass[] = ""; // Set to your password or "" for open networks.


#define EspSerial Serial1

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

float X, Y, Z;
boolean wokeUp = false; //Indicates if the eater already woke up

void setup() {
  
  SerialUSB.begin(115200);
  CircuitPlayground.begin();

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
}

void loop() {
  
  Blynk.run(); // Handle communication with Blynk server
  if (!wokeUp && CircuitPlayground.lightSensor() > 135) // Check if the eater hasn't woken up yet and the light level indicates the the light in the room turned on
  {
      Blynk.virtualWrite(V0, 1); // Send a "virtual write" to the Blynk server to indiciate that the eater woke up. 
      wokeUp = true;
  }
  
}

// This function loops and checks whether the door is being open, exits the loop once it detects the door was opened.
void checkDoor(){
  X = CircuitPlayground.motionX(); // Read the X-axis motion sensor value
  Z = CircuitPlayground.motionZ(); // Read the Z-axis motion sensor value
  while (!(X < -1.8 && Z < -2.8))
  {
    Blynk.run();
    X = CircuitPlayground.motionX(); // Updates the X-axis motion sensor value
    Z = CircuitPlayground.motionZ(); // Updates the Z-axis motion sensor value
  }
  Blynk.virtualWrite(V2, 1); // Send a "virtual write" to the Blynk server to indicate that the man opened the door
  
}

// This function is called whenever the value of virtual pin V4 on the Blynk server changes
BLYNK_WRITE(V4){
  int sound = param.asInt(); // Read the new value of virtual pin V4
  if(sound == 1) // Check if the value is equal to 1
  {
    turnOnTV(); // Call the turnOnTV function
    checkDoor(); // Call the checkDoor function
  }
}

// This function sends an infrared signal to turn on a TV
void turnOnTV(){
  CircuitPlayground.irSend.send(MY_PROTOCOL, MY_POWER, MY_BITS); // Send the infrared signal
}
