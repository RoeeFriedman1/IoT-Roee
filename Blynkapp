/*
  Blynk App
  This code is working with the Blynk app and a CFX board. 
  The user can access data about the room in which the CFX is in, such as light level, sound level, and temperature. 
  Additionally, it allows the user to send commands to the device, such as turning on the "flash light" (flash light mode, 
  playing a note or playing a scary song (scare people mode).
  
  Created By:
  Roee Friedman 
  Ido Bosmi 
*/ 


#include <Adafruit_CircuitPlayground.h>


// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLnTwayMb3"
#define BLYNK_DEVICE_NAME           "blynksensor"
#define BLYNK_AUTH_TOKEN            "dhTsjG4SAUQFWc0J21yrWLPK645EJmZ8"



// Comment this out to disable prints and save space
#define BLYNK_PRINT SerialUSB


#define NOTE_E4 329.63
#define NOTE_FS4 369.99
#define NOTE_G4 392
#define NOTE_A4 440

//Notes, duration and tempo for Scary Song
float notes[] = {NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4,
                 NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4};
int beats [] = {4, 4, 4, 4, 2, 2, 2, 2, 2, 2,
                4, 4, 4, 4, 2, 2, 2, 2, 2, 2};
int tempo = 50;

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "RUNI-Dorms";
char pass[] = "";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1


// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

void setup()
{
  // Debug console
  SerialUSB.begin(115200);
  CircuitPlayground.begin();

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
}

unsigned long lastSent;
int i =0;

void loop()
{
  Blynk.run();
  if(millis() - lastSent > 1000){

    //Gets the light level and send it to virtual pin V0
    int lightLevel = CircuitPlayground.lightSensor();
    Blynk.virtualWrite(V0, lightLevel);

    //Gets the temperature and send it to virtual pin V1
    float temperature = CircuitPlayground.temperature();
    Blynk.virtualWrite(V1, temperature);

    //Gets the sound level and send it to virtual pin V2
    int soundLevel = CircuitPlayground.mic.soundPressureLevel(20);
    Blynk.virtualWrite(V2, soundLevel-55);
    lastSent = millis();
  }
}

// In Blynk app writes values to the Virtual Pin V3
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V3 to a variable

  //Change color to white the first "pinValue" pixels, for the rest change color to black 
  for(int i = 0; i <= 10; i++)
  {
    if(i < pinValue)
    {
       CircuitPlayground.setPixelColor(i, 255, 255, 255);
    } 
    else
    {
      CircuitPlayground.setPixelColor(i, 0, 0, 0);
    }
       
  }
}


// In Blynk app writes values to the Virtual Pin V4
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V4 to a variable

  if(pinValue == 1)
  {
      // Play the Scary Song
      for (int i = 0; i < 20; i++)
      { 
          CircuitPlayground.playTone(notes[i], beats[i] * tempo);
          delay(tempo / 2);
      }

       //Change the value of V4 back to 0
       Blynk.virtualWrite(V4, 0);
  }
}



// In Blynk app writes values to the Virtual Pin V5
BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  
  if(pinValue == 1)
  {
    //playNote for 2 seconds
    CircuitPlayground.playTone(440, 2000);
    
    //Change the value of V5 back to 0
    Blynk.virtualWrite(V5, 0);
  }
}
