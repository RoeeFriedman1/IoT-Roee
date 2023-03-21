/*
  Imaginary Pet
  We have created an imaginary animal that uses light, sound, touch (capacitance), and accelerometer sensors to react to events such as falling,
  touching, and shouting by making different sounds using the "Pink Trombone" website and mouse control.

  The circuit:
  * Inputs:
    * Aluminum foil used as a conductor to connect to input A1 
  
  * Computer with "https://dood.al/pinktrombone/" open in its browser, connected using micro USB cable 
     
  
  Created By:
  Roee Friedman 
  Ido Bosmi 
*/ 

#include <Adafruit_CircuitPlayground.h>
#include <Mouse.h>
#include <Wire.h>
#include <SPI.h>




//This code is designed for a display settings of 1920X1080 and 125%

long X,Y,Z;
float sound, vec;
bool isAlwaysVoiceOn = false, isDark = false;
int LIGHT_RECOVERY_TIME = 5000; //The time that takes to "recover" from light exposure after darkness. 

unsigned long startTime, currentTime;

void setup() {
  CircuitPlayground.begin();
  Mouse.begin();
}

void loop() {
  
  if (!CircuitPlayground.slideSwitch())
  {
    return;
  }
  
  
  // If it's dark
  if(CircuitPlayground.lightSensor() < 10)
  { 
      isDark = true; // initialize isDark to true.
  }
  
  //If the light exposure is high after darkness
  if(CircuitPlayground.lightSensor() >= 50 && isDark)
  { 
     wakeUpRutine();
  }
  
  //If sensing touch
  if(CircuitPlayground.readCap(6) >= 1016)
  { 
      pleasureSound();
  }
  
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();
  vec = sqrt(X*X + Y*Y + Z*Z);
  
  if(vec < 1)
  {
    falling();
  }
  
  sound = CircuitPlayground.mic.soundPressureLevel(1);
  
  if(sound > 70)
  {
    mimicDuringFight();
  }
  

}

void setMouseAbsolute(int x, int y) {
  // reset location to 0,0
  for (int i = 0; i < 50; ++i) {
    Mouse.move(-50, -50);
  }
  while (x >= 50) {
    Mouse.move(50, 0);
    x -= 50;
  }
  if (x > 0) {
    Mouse.move(x, 0);
  }
  while (y >= 50) {
    Mouse.move(0, 50);
    y -= 50;
  }
  if (y > 0) {
    Mouse.move(0, y);
  }
}

void dragMouseRelative(int deltax, int deltay, int interval) {
  if (deltax == 0) {
    Mouse.press(MOUSE_LEFT);
    for (long i = 0; i < abs(deltay); ++i) {
      Mouse.move(0, sign(deltay));
    }
    Mouse.release(MOUSE_LEFT);
  }
  else if (deltay == 0) {
    Mouse.press(MOUSE_LEFT);
    for (long i = 0; i < abs(deltax); ++i) {
      Mouse.move(sign(deltax), 0);
    }
    Mouse.release(MOUSE_LEFT);
  }
  else {
    long steps = abs(deltax) * abs(deltay);
    Mouse.press(MOUSE_LEFT);
    for (long i = 0; i < steps; ++i) {
      if (i % abs(deltay) == 0) {
        Mouse.move(sign(deltax), 0);
      }
      if (i % abs(deltax) == 0) {
        Mouse.move(0, sign(deltay));
      }
      if (interval) {
        delayMicroseconds(interval);
      }
    }
  }
}


int sign(int n) {
  return n == 0 ? 0 : (n > 0 ? 1 : -1);
}

void falling(){
  
  // Wait until the Arduino lands after falling
  while(vec < 1)
  { 
    X = CircuitPlayground.motionX();
    Y = CircuitPlayground.motionY();
    Z = CircuitPlayground.motionZ();
    vec = sqrt(X*X + Y*Y + Z*Z);
  }
  
  //After landing
  
  //Set tounge control
  setMouseAbsolute(340, 233);
  Mouse.click(MOUSE_LEFT);
  
  //set voicebox control
  setMouseAbsolute(630, 320);
  Mouse.press(MOUSE_LEFT);
  
  delay(3000);
  
  Mouse.release(MOUSE_LEFT);
}

void mimicDuringFight(){
  
  //set voicebox control
  setMouseAbsolute(630, 320);
  Mouse.click(MOUSE_LEFT);
  
  //Turn on "Always Voice"
  clickAlwaysVoice();
  
  //Set Oral Cavity
  setMouseAbsolute(320, 230);
  
  //Loop while sound is greater than 70
  while(sound > 70)
  {
    //set tounge
    dragMouseRelative(-100, -100, 10);
    dragMouseRelative(100, 100, 10);
    sound = CircuitPlayground.mic.soundPressureLevel(1);
  }
  
  Mouse.release(MOUSE_LEFT);
  
  // Turn off "Always Voice" if it is on
  if(isAlwaysVoiceOn)
  {
    clickAlwaysVoice();
  }

}

void pleasureSound(){
  
  //set voicebox control
  setMouseAbsolute(190, 325);
  Mouse.click(MOUSE_LEFT);
  
  //Set tounge control
  setMouseAbsolute(340, 240);
  Mouse.click(MOUSE_LEFT);
  
  //Turn on "Always Voice"
  clickAlwaysVoice();
  
  //Set Oral Cavity
  setMouseAbsolute(340, 150);
  Mouse.press(MOUSE_LEFT);
  
  
  startTime = millis();
  currentTime = millis();
  
  // Loop while the time elapsed is less than 500 milliseconds since a touch is detected
  while(startTime > currentTime - 500)
  {
    if(CircuitPlayground.readCap(6)>=1016)
    {
      startTime = millis();
    }
    currentTime = millis();
  }
  Mouse.release(MOUSE_LEFT);
  
  // Turn off "Always Voice"  if it is on
  if(isAlwaysVoiceOn)
  {
    clickAlwaysVoice();
  }
   

}

void wakeUpRutine(){
  
  clickAlwaysVoice();
  
  // Set voice box control
  setMouseAbsolute(190, 320); 
  Mouse.click(MOUSE_LEFT);
  
  // Set tongue control
  setMouseAbsolute(330, 245); 
  Mouse.click(MOUSE_LEFT);
  
  // Set Oral Cavity
  setMouseAbsolute(265, 215);
  
  Mouse.press(MOUSE_LEFT);
  
  startTime = millis(); //Save the time of the light exposure.
  currentTime = millis(); //Save the time of the light exposure.
  
  // Loop while light sensor value is between 50 and 100 and within the time limit
  while(CircuitPlayground.lightSensor() >= 50 && CircuitPlayground.lightSensor() < 100 && startTime > currentTime - LIGHT_RECOVERY_TIME)
  {
    currentTime = millis(); // Update to current time.
  }
  
  // Check if the loop was exited due to the light sensor value being greater than 100
  if(CircuitPlayground.lightSensor() > 100)
  { 
    startTime = millis(); // Update startTime to current time.
    Mouse.release(MOUSE_LEFT);
    setMouseAbsolute(275, 320); // set voice box control
    Mouse.click(MOUSE_LEFT);
    setMouseAbsolute(330, 230); // set Tongue control
    Mouse.click(MOUSE_LEFT);
    setMouseAbsolute(265, 215); //set Oral Cavity
    Mouse.press(MOUSE_LEFT);
  }
  
  // Loop while light sensor value is between 100 and 400 and within the time limit
  while(CircuitPlayground.lightSensor() >= 100 && CircuitPlayground.lightSensor() < 400 && startTime > currentTime - LIGHT_RECOVERY_TIME)
  {
    currentTime = millis(); // Update to current time.
  }
  
  // Check if the loop was exited due to the light sensor value being greater than 400
  if(CircuitPlayground.lightSensor() > 400)
  {
    startTime = millis(); // Update startTime to current time.
    Mouse.release(MOUSE_LEFT);
    setMouseAbsolute(380, 320); // set voice box control
    Mouse.click(MOUSE_LEFT);
    setMouseAbsolute(330, 215); // set Tongue control
    Mouse.click(MOUSE_LEFT);
    setMouseAbsolute(265, 215); // set Oral Cavity
    Mouse.press(MOUSE_LEFT);
    
  }
  
  // Loop while light sensor value is between 400 and 700 and within the time limit
  while(CircuitPlayground.lightSensor() >= 400 && CircuitPlayground.lightSensor() < 700 && startTime > currentTime - LIGHT_RECOVERY_TIME){
    currentTime = millis(); // Update to current time.
  }
  
  // Check if the loop was exited due to the light sensor value being greater than 700
  if(CircuitPlayground.lightSensor() > 700)
  {
    startTime = millis(); // Update startTime to current time.
    Mouse.release(MOUSE_LEFT);
    setMouseAbsolute(380, 320); // set voice box control
    Mouse.click(MOUSE_LEFT);
    setMouseAbsolute(330, 245); //set Tongue control
    Mouse.click(MOUSE_LEFT);
    setMouseAbsolute(265, 215); // set Oral Cavity
    Mouse.press(MOUSE_LEFT);
  }
  
  // Loop while light sensor value is geater than 700 and within the time limit
  while(CircuitPlayground.lightSensor() > 700 && startTime > currentTime - LIGHT_RECOVERY_TIME)
  {
    currentTime = millis(); // Update to current time.
  }
  
  isDark = false;
  Mouse.release(MOUSE_LEFT);
  clickAlwaysVoice(); //turn off "Always Voice"
}


void clickAlwaysVoice(){
  setMouseAbsolute(480, 280);
  Mouse.click(MOUSE_LEFT);
  isAlwaysVoiceOn = !isAlwaysVoiceOn;
}
