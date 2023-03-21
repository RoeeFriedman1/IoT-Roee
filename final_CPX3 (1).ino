/*
  CPX3
      This sketch represents the CPX3 that is located in the kitchen, which is connected to the safe box in a electric circuit. 
      Also, it is connected to a Blynk device that includes a Simon game in the Blynk app.  

  The circuit:
  * Coins attached with foil and crocodile clips to pins A1, A2, A3, A4, A5

  Created By:
  Roee Friedman 208743583
  Ido Bosmi 209794395
*/ 



#include <Adafruit_CircuitPlayground.h>

#include <QList.h>


#define BLYNK_TEMPLATE_ID "XXXXXXXXXXX" // Insert your BLYNK_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME "XXX" // Insert your BLYNK_TEMPLATE_NAME
#define BLYNK_AUTH_TOKEN "XXXXXXXXXXXXXXXXXXXXXXXXXXXX" // Insert your BLYNK_AUTH_TOKEN

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
char ssid[] = "XXX"; // Set to your netwrok name.
char pass[] = ""; // Set to your password or "" for open networks.

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


String code = "1111"; // Default password
String input ="";


boolean isPasswordMode = false; // Indicated if the user can try passwords

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
  CircuitPlayground.begin();

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  
  Blynk.virtualWrite(V6, 0); // Initialize the score of the game to 0 
}


void loop() {

    Blynk.run();

    if(!isPasswordMode)
    {
      if(millis() - lastSent > 1000)
      {
        if(!userTurn)
        {
            addRandomNote();
        }
        lastSent = millis();
      }
    }
    
    else // The user can try enter the password.
    {
      
      if(CircuitPlayground.readCap(6) > 1000) // If the button that represents 1 was pressed
      { 
        input = input + "1"; // Add 1 to the current input
        for(int i=0; i<10; i++) // Go over all the newPixels
        {
          CircuitPlayground.setPixelColor(i, 255, 255, 255); // Color the current neoPixel white.
        }
        delay(300);
        CircuitPlayground.clearPixels(); // Turn off all the neoPixels.
      }
   
      if(CircuitPlayground.readCap(9) > 1000) // If the button that represents 2 was pressed
      {
        input = input + "2"; // Add 2 to the current input
        for(int i=0; i<10; i++) // Go over all the newPixels
        {
          CircuitPlayground.setPixelColor(i, 255, 255, 255); // Color the current neoPixel white.
        }
        delay(300);
        CircuitPlayground.clearPixels(); // Turn off all the neoPixels.
      }
 
      if(CircuitPlayground.readCap(2) > 1000)// If the button that represents 3 was pressed
      {
        input = input + "3"; // Add 3 to the current input
        for(int i=0; i<10; i++) // Go over all the newPixels
        {
          CircuitPlayground.setPixelColor(i, 255, 255, 255); // Color the current neoPixel white.
        }
        delay(300);
        CircuitPlayground.clearPixels(); // Turn off all the neoPixels.
      }
    
      if(CircuitPlayground.readCap(3) > 1000) // If the button that represents 1 was pressed
      {
        input = input + "4"; // Add 4 to the current input
        for(int i=0; i<10; i++) // Go over all the newPixels
        {
          CircuitPlayground.setPixelColor(i, 255, 255, 255); // Color the current neoPixel white.
        }
        delay(300);
        CircuitPlayground.clearPixels(); // Turn off all the neoPixels.
      }

      if(CircuitPlayground.readCap(10) > 1000) // If the button that represents SUBMIT was pressed
      {
        checkPass();
      }
    
    }
}

//Checks if the password is correct and act accordingly
void checkPass(){
  
  if(input.equals(code)) // If the input is correct
  {
    Blynk.virtualWrite(V8, 1); // Activate the webhook to send the new job status to the guard
    for(int i=0; i<10; i++) // Go over all the neoPixels
    {
        CircuitPlayground.setPixelColor(i, 0, 255, 0); // Colors the current neoPixel green
    }
    playSuccess(); 
  }
  else
  {
    for(int i=0; i<10; i++) // Go over all the neoPixels
    {
        CircuitPlayground.setPixelColor(i, 255, 0, 0); // Colors the current neoPixel red
    }
    playWrong();
    CircuitPlayground.clearPixels(); // Turn off all the neoPixels.
    input = ""; // Reset the input
  }
}

// Play using the speaker 3 notes that indicates that the user entered the correct password
void playSuccess(){
  CircuitPlayground.playTone(261.63, 100);
  CircuitPlayground.playTone(392, 80);
  CircuitPlayground.playTone(523.25, 100);
}

// Play using the speaker 3 notes that indicates that the user entered the wrong password
void playWrong(){
  CircuitPlayground.playTone(174.61, 150);
  CircuitPlayground.playTone(164.81, 150);
  CircuitPlayground.playTone(155.56, 200);
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


// This function is called whenever the value of virtual pin V0 on the Blynk server changes
BLYNK_WRITE(V0){
int pinValue = param.asInt(); // Assigning incoming value from pin V0 to a variable
  if(pinValue == 1) // Check if the value is equal to 1
  {
    userMove(0);
  }
}

// This function is called whenever the value of virtual pin V1 on the Blynk server changes
BLYNK_WRITE(V1){
int pinValue = param.asInt(); // Read the new value of virtual pin V1
  if(pinValue == 1) // Check if the value is equal to 1
  {
    userMove(1);
  }
}

// This function is called whenever the value of virtual pin V2 on the Blynk server changes
BLYNK_WRITE(V2){
int pinValue = param.asInt(); // Read the new value of virtual pin V2
  if(pinValue == 1) // Check if the value is equal to 1
  {
    userMove(2);
  }
}

// This function is called whenever the value of virtual pin V3 on the Blynk server changes
BLYNK_WRITE(V3) {
int pinValue = param.asInt(); // Read the new value of virtual pin V3
  if(pinValue == 1) // Check if the value is equal to 1
  {
    userMove(3); 
  }
}

// This function is called whenever the value of virtual pin V4 on the Blynk server changes
BLYNK_WRITE(V4){
  int pinValue = param.asInt(); // Read the new value of virtual pin V4
  if(pinValue == 1) // Check if the value is equal to 1
  {
    userMove(4);
  }
}

// This function is called whenever the value of virtual pin V5 on the Blynk server changes
BLYNK_WRITE(V5){
  int pinValue = param.asInt(); // Read the new value of virtual pin V5
   if(pinValue == 1) // Check if the value is equal to 1
   {
      gameOver();
   }
}

// This function is called whenever the value of virtual pin V7 on the Blynk server changes
BLYNK_WRITE(V7){
  code = param.asStr(); 
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

// Reset everything and start over the Simon game
void gameOver(){
  currLength = 1; // Reset currLength to its original value
  userTurn = false;
  currentNotes.clear(); //Clears the note sequence of the currentStage
  Blynk.virtualWrite(V6, 0); // Updates the score to zero
  delay(1000);
}

// Updates everything and starts the next stage in the Simon game
void nextStage(){
  currLength = currLength + 1; // Updates currentLength, the number of notes in notes sequence of the new stage 
  if(currLength == 4){
    Blynk.virtualWrite(V6, currLength-1); // Updates the score
    Blynk.virtualWrite(V8, 2); // Activate the webhook to send the password via email
    isPasswordMode = true;
  }
  else {
    userTurn = false;
    Blynk.virtualWrite(V6, currLength-1); // Updates the score
    delay(1000);
  }
}
