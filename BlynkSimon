/*
  BlynkSimon App
  We built a Simon game using CFX and Blynk, where CFX randomly selects and plays notes while also sending a command to the Blynk app to turn on and off the corresponding button,
  Also, the CFX sends the current score each stage to the Blynk app.
  The Blynk app sends the notes the user is playing to the CFX, that plays them and checks the correctness.

  Video link: https://youtu.be/a250JbGYilw
  
  Created By:
  Roee Friedman 208743583
  Ido Bosmi 209794395
*/ 


#include <Adafruit_CircuitPlayground.h>
#include <QList.h>


// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLvN6qikke"
#define BLYNK_DEVICE_NAME "Blynk2"
#define BLYNK_AUTH_TOKEN "6Fun127Jk_Y380Gq67knEUaK5oXHNS0i"

// Comment this out to disable prints and save space
#define BLYNK_PRINT SerialUSB

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Runi-Dorms";
char pass[] = "";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

float notes[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4}; //Holds the notes that are part of the game 

QList<int> currentNotes; // At any given time during the game holds the current sequence of notes.

boolean userTurn = false;
int currLength = 1; // Indicates the current stage and number of notes in it
int userIndex; 

unsigned long lastSent;

void setup()
{
  // Debug console
  SerialUSB.begin(115200);
  CircuitPlayground.begin();

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  
  Blynk.virtualWrite(V6, 0); // Initialize the score of the game to 0 
}

void loop()
{
  Blynk.run();
  if(millis() - lastSent > 1000){
    
    if(!userTurn){
        addRandomNote();
    }
    lastSent = millis();
  }
}

// This function adds a random note to the currentNotes queue
void addRandomNote(){
  
    int randomNote = random(5); // Generate a random number between 0 and 4
    currentNotes.push_front(randomNote); // Add the random number to the front of the currentNotes queue
    playNotes();
    userIndex = currLength - 1;
    userTurn = true; 
}

// Play all the notes in the currentNotes queue and turn on and off the corresponding button
void playNotes(){

  for(int i = currLength-1; i >= 0; i--)
  {
    int note = currentNotes[i]; 
    CircuitPlayground.playTone(notes[note], 200); // Gets the frequency of the current note and play the note

    // Turn on and off the button corresponding to the current note
    switch(note){
      case 0: Blynk.virtualWrite(V0, 1);
              Blynk.virtualWrite(V0, 0);
              break;
      case 1: Blynk.virtualWrite(V1, 1);
              Blynk.virtualWrite(V1, 0);
              break;
      case 2: Blynk.virtualWrite(V2, 1);
              Blynk.virtualWrite(V2, 0);
              break;
      case 3: Blynk.virtualWrite(V3, 1);
              Blynk.virtualWrite(V3, 0);
              break;
      case 4: Blynk.virtualWrite(V4, 1);
              Blynk.virtualWrite(V4, 0);
              break;                    
    }
  }
}


//In Blynk app writes values to the Virtual Pin V0
BLYNK_WRITE(V0){
int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  if(pinValue == 1)
  {
    userMove(0);
  }
}

//In Blynk app writes values to the Virtual Pin V1
BLYNK_WRITE(V1){
int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  if(pinValue == 1)
  {
    userMove(1);
  }
}

//In Blynk app writes values to the Virtual Pin V2
BLYNK_WRITE(V2){
int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  if(pinValue == 1)
  {
    userMove(2);
  }
}

//In Blynk app writes values to the Virtual Pin V3
BLYNK_WRITE(V3) {
int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  if(pinValue == 1)
  {
    userMove(3); 
  }
}

///In Blynk app writes values to the Virtual Pin V4
BLYNK_WRITE(V4){
  int pinValue = param.asInt(); // Assigning incoming value from pin V5 to a variable
  if(pinValue == 1)
  {
    userMove(4);
  }
}

//In Blynk app writes values to the Virtual Pin V5
BLYNK_WRITE(V5){
  int pinValue = param.asInt();
   if(pinValue == 1)
   {
      gameOver();
   }
}


/** Gets the note the user just played and checks if he was right or wrong. 
*   If the user got the note right, continue.
*   If this was the last note of the sequence, continue to the next stage
*   If the user was wrong, run gameOver()
*/
void userMove(int note){
  if(userTurn){
    if (userIndex >= 0) 
    {
        if (currentNotes[userIndex] == note) // Check if the current note is 3
        { 
            CircuitPlayground.playTone(notes[note], 200); // Play the pressed note
            userIndex = userIndex - 1; // Decrement the userIndex variable
            if (userIndex < 0) // Check if this was the last note in the current stage
            { 
                nextStage();
            }
        } 
        else // If the current note in the sequence is not [note], the user input is incorrect
        {
            gameOver(); // Call the gameOver() function
        }
    }
  }
}

// Reset everything and start over the game
void gameOver(){
  currLength = 1; // Reset currLength to its original value
  userTurn = false;
  currentNotes.clear(); //Clears the note sequence of the currentStage
  Blynk.virtualWrite(V6, 0); // Updates the score to zero
  delay(1000);
}

// Updates everything and starts the next stage
void nextStage(){
  currLength = currLength + 1; // Updates currentLength, the number of notes in notes sequence of the new stage 
  userTurn = false;
  Blynk.virtualWrite(V6, currLength-1); // Updates the score
  delay(1000);
}
