/*
Music Gloves

We created interactive and fun gloves that play music notes.
To implement our idea we used 2 sensors - light sensor and accelerometer (motion). Additionally, we utilized both the right and left buttons 
as well as the slide switch to control our device actions. We also used the neoPixels to indicate the different notes and when we turn on or off the device.

  Video link: https://www.youtube.com/watch?v=omzZyfckoDM&ab_channel=RoeeFriedman
  Created By:
  Roee Friedman 
  Ido Bosmi 
*/ 


#include <Adafruit_CircuitPlayground.h>
float X,Z; //Hold the current value of the movement X plain and Z plain accordingly (motionX, motionZ.
float rightHand[4] = {261.63, 293.66, 329.63, 493.88}; // The notes frequencies (Hz) that will be played by the right hand (C,D,E and B.
float leftHand[4] = {349.3, 392, 440, 523.23}; // The notes frequencies (Hz) that will be played by the left hand (F,G,A and C).
int rightColor[4][4]= {{255, 51 , 153}, {255, 255, 0}, {0, 255, 255}, {255, 0, 0}}; // Right hand newPixels of the notes.
int leftColor[4][4]= {{0, 0, 255}, {255, 255, 255}, {0, 255, 0}, {102, 0, 102}};// Left hand newPixels of the notes.
bool isRight = true; // Indicates whether the Arduino represents a right (true) or left (false) hand.
bool isOn = false; // Indicates if the device is on (true) or off (false)
float octave = 1; // Indicates the current octave of the notes.
bool octaveChange = false; // Indicates whether the device is currently in an octave changing process.

void setup() {
  CircuitPlayground.begin();  // Initialize the CP library.
}

void loop() {
    
    if(CircuitPlayground.slideSwitch()){ //Checks if slide switch is on to turn on the device.
      turnOn();
    } else if (isOn) { // Else, checks if the current mode of the device is on, if so, turn it off.
      turnOff();
    }
    while(CircuitPlayground.slideSwitch() && isOn){ //While the slide switch in on and also the turn on process is complete.
      
      //Determines the hand you want to play with and change the redLed mode to indicate it.
      if(CircuitPlayground.rightButton()){
        isRight = true;
        CircuitPlayground.redLED(true);
      }
      
      if(CircuitPlayground.leftButton()){
        isRight = false;
        CircuitPlayground.redLED(false);
      }

       // Gets the current motionX and motionZ to determine the note to play.
       X = CircuitPlayground.motionX();
       Z = CircuitPlayground.motionZ();

      
      //Play C or F according to the current hand and change the neoPixels color according to the note.
      while(X < -4 && X > -10 && isOn){
        isRight ? playNote(rightHand[0]) : playNote(leftHand[0]);
        isRight ? setColor(rightColor[0]) : setColor(leftColor[0]);
        X = CircuitPlayground.motionX();
      }
      //Play D or G according to the current hand and change the neoPixels color according to the note.  
      while(X > 4 && X < 10 && isOn){
        isRight ? playNote(rightHand[1]) : playNote(leftHand[1]);
        isRight ? setColor(rightColor[1]) : setColor(leftColor[1]) ;
        X = CircuitPlayground.motionX();
      }
      //Play E or A according to the current hand and change the neoPixels color according to the note.
      while( Z > 8 && Z < 13 && isOn){
        isRight ? playNote(rightHand[2]) : playNote(leftHand[2]);
        isRight ? setColor(rightColor[2]) : setColor(leftColor[2]); 
        Z = CircuitPlayground.motionZ();
      }

      //Play B or C according to the current hand and change the neoPixels color according to the note.
      while(CircuitPlayground.lightSensor() < 15){
        isRight ? playNote(rightHand[3]) : playNote(leftHand[3]);
        isRight ? setHalfColor(rightColor[3]) : setHalfColor(leftColor[3]);
      }

      if(CircuitPlayground.lightSensor() < 700 && CircuitPlayground.lightSensor() > 250 && !octaveChange && isOn){
        // If the device gets medium light (250 - 700, 1 line in iPhone flashlight) and we are not currently in the middle of octave change, go 1 octave down.
        octave = octave * 0.5;
        octaveChange = true;
        
      }
      if(CircuitPlayground.lightSensor() > 700 && !octaveChange && isOn){
        // if the device gets strong light (>700, 4 line in iPhone flashlight) and we are not currently in the middle of octave change, go 1 octave up.
        octave = octave * 2;
        octaveChange = true;
      }
      if(CircuitPlayground.lightSensor() > 30 && CircuitPlayground.lightSensor() < 100 ){
        // If the light is normal, it means that we finished the octave change process.
        octaveChange = false;
      }
      
      CircuitPlayground.clearPixels(); //If no note is currently playing, clear all the pixels.
    }

}


void setColor(int color[3]){
  // The function gets an array of length 3 that represents a color. Change the colors of all the neoPixels to that color.
  for(int i = 0; i <= 9; i++){
    CircuitPlayground.setPixelColor(i, color[0], color[1], color[2]);
  }
}

void setHalfColor(int color[3]){
  // the function gets an array of length 3 that represents a color. Change the colors of neoPixels 3-7 to that color (to not interfere with the light sensor.
  for(int i = 3; i <= 7; i++){
    CircuitPlayground.setPixelColor(i, color[0], color[1], color[2]);
  }
}

void turnOn(){
  //Does the turnOn theme - turn on the white color on each neoPixel one by one until all of them are on, then turn all of them off and change the mode of turnOn.
  for(int i = 0; i <= 9; i++){
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(100);
  }
  CircuitPlayground.clearPixels();
  isOn = true;
}

void turnOff(){
  //Does the turnOff theme - turn on the white color on each neoPixel one by one separately. Then, all of them together.
  //Then turn all of them off and change the mode of turnOn to false and reset all the variables to their default.
  for(int i = 9; i >= 0; i--){
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(100);
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
  for(int i = 9; i >= 0; i--){
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
  }
  delay(300);
  CircuitPlayground.clearPixels();
  CircuitPlayground.redLED(false);
  isRight = true;
  isOn = false;
  octave = 1;
  octaveChange = false;
}

void playNote(float note){
  // play the given note using playTone function for 10 milliseconds. The octave of the note will be determined by the current octave value.
  CircuitPlayground.playTone(note * octave, 10);
}
