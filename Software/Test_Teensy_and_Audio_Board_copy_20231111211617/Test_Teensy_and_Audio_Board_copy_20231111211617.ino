/* 
  North Coast Modular Collective Ann Arbor Drums
  Version .001 Dec 2017
  ---------------------------------------------------------
  Copyright 2017 North Coast Modular Collective
  MIT License:
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
  documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and 
  to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of 
  the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  ----------------------------------------------------------  

  This sketch is intended to be used as a basic health check for a teensy 3.2 with an audio board attached to it.
*/

//For modifications, the code from the "GUItool" below can be imported into the Teensy audio system design tool at https://www.pjrc.com/teensy/gui/index.html

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum1;          //xy=352.0000305175781,220.00009155273438
AudioOutputI2S           i2s2;           //xy=706,246
AudioConnection          patchCord1(drum1, 0, i2s2, 0);
AudioConnection          patchCord2(drum1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=518.25,408.75
// GUItool: end automatically generated code

AudioControlSGTL5000 codec;

void setup() {
// Get the board ready
  AudioMemory(200);
  codec.enable();
  codec.volume(1); //0-1. crank it. We'll control volume per drum channel with the mixers.
  codec.lineOutLevel(13); //make the codec use the maximum voltage range for output (3.16 volts p-p)
  codec.dacVolume(1);
 
  AudioNoInterrupts(); 
//Set up drum1
  drum1.frequency(440);
  drum1.length(500);
  drum1.secondMix(0.0);
  drum1.pitchMod(0.50);

  AudioInterrupts();
} //End of setup()

elapsedMillis msec = 0; //here for timing the troubleshooting loop at the bottom

void loop() {  //Main loop for the run state

// Just loop a drum hit to make sure the Teensy board and audio sheild function.
  if (msec >= 500) {
      drum1.noteOn();   //Play the drum1 sound just once
      msec = 0;
  } else {
    //chill and do nothing
  }
    
} //End of main Loop
