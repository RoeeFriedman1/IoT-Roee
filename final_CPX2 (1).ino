/*
  CPX2
      This sketch represents the CPX2 located in the guard's office, which alerts the guard about the night eater status. 
      It communicates with CPX1 through Blynk.

  Created By:
  Roee Friedman 208743583
  Ido Bosmi 209794395
*/ 

#include <Adafruit_CircuitPlayground.h>

#define BLYNK_TEMPLATE_ID "XXXXXXXXXXX" // Insert your BLYNK_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME "XXX" // Insert your BLYNK_TEMPLATE_NAME
#define BLYNK_AUTH_TOKEN "XXXXXXXXXXXXXXXXXXXXXXXXXXXX" // Insert your BLYNK_AUTH_TOKEN

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;


// Your WiFi credentials.
char ssid[] = "XXX"; // Set to your netwrok name.
char pass[] = ""; // Set to your password or "" for open networks.

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349

// Notes for the "Who is talking to me?" (Free Translation), by Gur and Oah.
float notes[] = {NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_E4};
int beats [] = {275, 275, 275, 80, 80, 80, 80, 275, 275, 275, 80, 80, 80, 80, 80};

void setup() {
  
  SerialUSB.begin(115200);
  CircuitPlayground.begin();

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
}
void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run(); // Handle communication with Blynk server
}

// This function is called whenever the value of virtual pin V3 on the Blynk server changes
BLYNK_WRITE(V3){
  int light = param.asInt(); // Read the new value of virtual pin V3
  if(light == 1) // Check if the value is equal to 1
  {
    playAlarmSong(); // Call the playAlarmSong function
    listen(); // Call the listen function
  }
}
// This function is called whenever the value of virtual pin V5 on the Blynk server changes
BLYNK_WRITE(V5){
  int door = param.asInt(); // Read the new value of virtual pin V5
  if(door == 1) // Check if the value is equal to 1
  {
    alarmLeds(); // Call the alarmLeds function
  }
}

// This function listens for a loud sound and sends a virtual write to the Blynk server if the sound is detected
void listen(){
  int sound = CircuitPlayground.mic.soundPressureLevel(10); // Read the sound pressure level from the microphone
  while(sound < 110) // Check if the sound level is below a certain threshold
  {
    sound = CircuitPlayground.mic.soundPressureLevel(10); // Updates sound
  }
  Blynk.virtualWrite(V1, 1); // Send a "virtual write" to the Blynk server to indicate that the guard shouted.
  
}


void alarmLeds(){
  for(int j=0; j<10; j++) // Repeat 10 times
  {
    for(int i = 9; i >= 0; i--) // Loop through all 10 LEDs
    {
      Blynk.run(); 
      CircuitPlayground.setPixelColor(i, 255, 0, 0); // Set the color of the current LED to red
      delay(50);
      CircuitPlayground.setPixelColor(i, 0, 0, 0); // Turn off the current LED
    }
  }
}

// This function plays the song by Gur and Oah on the speaker
void playAlarmSong(){
  for (int i = 0; i < 15; i++) // Loop through each note in the song
   { 
       CircuitPlayground.playTone(notes[i], beats[i]); // Play the current note
   }
}
