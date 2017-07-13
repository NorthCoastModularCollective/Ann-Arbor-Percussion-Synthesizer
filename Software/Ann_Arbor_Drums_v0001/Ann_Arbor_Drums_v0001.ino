/* 
  North Coast Modular Collective Ann Arbor Drums
  Version .001 July 2017
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
*/

//For modifications, the code from the "GUItool" below can be imported into the Teensy audio system design tool at https://www.pjrc.com/teensy/gui/index.html
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=126.7500228881836,411.7500915527344
AudioSynthWaveformSineModulated sine_fm1;       //xy=296.7500686645508,445.7500915527344
AudioSynthWaveform       waveform1;      //xy=298.50000762939453,341.00008392333984
AudioSynthNoiseWhite     noise1;         //xy=300.0000228881836,506.0000352859497
AudioSynthWaveformPWM    pwm1;           //xy=302.7500457763672,389.75006675720215
AudioSynthSimpleDrum     drum1;          //xy=306.0000305175781,291.00007820129395
AudioMixer4              mixer3;         //xy=473.7500228881836,347.0000343322754
AudioEffectEnvelope      envelope2;      //xy=618.0000381469727,458.0000066757202
AudioEffectEnvelope      envelope1;      //xy=620.0000762939453,346.99999618530273
AudioEffectBitcrusher    bitcrusher1;    //xy=771.0001831054688,347.00002670288086
AudioMixer4              mixer1;         //xy=929.0000686645508,365.99999618530273
AudioFilterStateVariable filter1;        //xy=1077.0000686645508,334.0000190734863
AudioMixer4              mixer2;         //xy=1221.0001640319824,363.0000801086426
AudioOutputI2S           i2s1;           //xy=1358.0000648498535,363.0000810623169
AudioConnection          patchCord1(sine1, pwm1);
AudioConnection          patchCord2(sine1, sine_fm1);
AudioConnection          patchCord3(sine_fm1, 0, mixer3, 3);
AudioConnection          patchCord4(waveform1, 0, mixer3, 1);
AudioConnection          patchCord5(noise1, envelope2);
AudioConnection          patchCord6(pwm1, 0, mixer3, 2);
AudioConnection          patchCord7(drum1, 0, mixer3, 0);
AudioConnection          patchCord8(mixer3, envelope1);
AudioConnection          patchCord9(envelope2, 0, mixer1, 1);
AudioConnection          patchCord10(envelope1, bitcrusher1);
AudioConnection          patchCord11(bitcrusher1, 0, mixer1, 0);
AudioConnection          patchCord12(mixer1, 0, filter1, 0);
AudioConnection          patchCord13(mixer1, 0, mixer2, 2);
AudioConnection          patchCord14(filter1, 0, mixer2, 0);
AudioConnection          patchCord15(filter1, 2, mixer2, 1);
AudioConnection          patchCord16(mixer2, 0, i2s1, 0);
AudioConnection          patchCord17(mixer2, 0, i2s1, 1);
// GUItool: end automatically generated code


AudioControlSGTL5000 codec;

//  Set up all the pin number variables
//trigger inputs
int Trigger1Pin = 5; 

//LEDs
//int Trig1LED = 9;

//Potentiometers
int pA2 = A1;
int pB2 = A0;
int pC2 = A2;
int pD2 = A7;
int pE2 = A6;

//variables for tracking the trigger states
int playTimes = 0; 

float Drum1Level = 1.0; //Audio level appears to require a float. see if this can be an int!!!!!!!!!!!!!!
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
Serial.begin(9600);

// Get the board ready
  AudioMemory(200);
  codec.enable();
  codec.volume(.4); //0-1. crank it. We'll control volume per drum channel with the mixers.
  
//Now set up the oscillators  
  AudioNoInterrupts(); 
//Set up drum1
  drum1.frequency(440);
  drum1.length(500);
  drum1.secondMix(0.0);
  drum1.pitchMod(0.50);
//Set up FX
  bitcrusher1.bits(16);
  bitcrusher1.sampleRate(44100);
//Set up noise sources
  noise1.amplitude(.5);
//Set up Envelopes
  envelope1.attack(.1);
  envelope1.decay(50);
  envelope1.hold(0);
  envelope1.sustain(0);
//  envelope3.release(0);
  envelope2.attack(.1);
  envelope2.decay(0);
  envelope2.hold(0);
  envelope2.sustain(0);
//Set up the mixers (for "Level" settings)
  mixer1.gain(0, 1); //gain(channel, level); just pass the audio through ("level" may be any floating point number from 0 to 32767)  
  mixer2.gain(0, 1); 
  mixer2.gain(1, 0);
  mixer2.gain(2, 0);
  mixer2.gain(3, 1);
  mixer3.gain(0, 1);
  mixer3.gain(1, 0);
  mixer3.gain(2, .5);
  mixer3.gain(3, 0);

//PWM
pwm1.frequency(440);
pwm1.amplitude(.9);

//Set up sine waves for LFO
sine1.amplitude(1);
sine1.frequency(0.1);

//FM
sine_fm1.amplitude(1);


//Set up Digital Pins
//triggers
  pinMode(Trigger1Pin, INPUT); //D1 is a digital input

//LEDs  
//  pinMode(Trig1LED, OUTPUT);

//Test Digital out
pinMode(1, OUTPUT); //only needed for testing (makes the CD output into a gate that can be patched into the trigger input on the board)

  AudioInterrupts();

/*
//Board ready, flash LEDs to indicate ready state on "boot up"
digitalWrite(Trig1LED, HIGH); //animate cycling the LEDs on from left to right
delay(100);              
digitalWrite(Trig1LED, LOW);
delay(100);              
digitalWrite(Trig1LED, HIGH);
delay(200);              
digitalWrite(Trig1LED, LOW);
*/

}

elapsedMillis msec = 0; //here for timing the troubleshooting loop at the bottom

void loop() {  //Main loop for the run state
//========LFO=====================
//The LFO might be working, but I can't get a good read or sample of it yet. 
float LFOLevel;
//sine1.frequency(map(analogRead(pD2), 0, 1023, 0.001, 100)); //set LFO frequency based on motion knob position


//=======PITCH KNOB=================
  int Drum1Freq = map(analogRead(pA2), 0, 1023, 20, 2500); 
  drum1.frequency(Drum1Freq);
  pwm1.frequency(Drum1Freq / 2);
  sine_fm1.frequency(Drum1Freq);


//===========SHAPE KNOB=================
//Set the shape of the envelope so that on the potentiomenter, left of half point is a ramp up (/|) 
//and right of half point is a decay slope (|\). 
  int Drum1Length = map(analogRead(pB2), 0, 1023, 1, 2000);
  if (Drum1Length > 1000) {  //right of center
  drum1.length(Drum1Length - 1000); 
  envelope1.attack(0);
  envelope1.delay(0);
  envelope1.hold(0);
  envelope1.decay(Drum1Length - 1000);
  envelope1.sustain(0);
  } else { //left of center
  drum1.length(abs(Drum1Length - 1000));   
  envelope1.sustain(0);
  envelope1.attack(abs(Drum1Length - 1000));  
  }
 
//=================TEMPURATURE KNOB==================

//Set modulation knob so that at center there is no effect, left of center is "colder" and right of center is "warmer"
  int ModLevel1 = map(analogRead(pC2), 0, 1023, 1, 2000); 
  mixer2.gain(0, mapf(ModLevel1, 1, 2000, 0, 1.0));
  mixer2.gain(1, mapf(ModLevel1, 1, 2000, 1.0, 0));

     
  filter1.frequency(abs(ModLevel1 - 1000)*4);
  
  if (ModLevel1 > 1000) { //right of center
    envelope2.decay(0);
    noise1.amplitude(0);
 //  filter1.frequency(map(abs(ModLevel1 - 1000), 1, 1000, 4410, 500));
   drum1.secondMix(mapf(abs(ModLevel1 - 1000), 1, 2000, 0, 1.0));

  } else { //left of center
    noise1.amplitude(mapf(abs(ModLevel1 - 1000), 1, 2000, .001, .2));
    envelope2.decay(map(abs(ModLevel1 - 1000), 1, 2000, 0, 70)); 
//  filter1.frequency(map(abs(ModLevel1 - 1000), 1, 2000, 1100, 4410));
  }


//============MOTION KNOB=================
int MotionLevel1 = map(analogRead(pD2), 0, 1023, 1, 2000);


if (MotionLevel1 > 1000) { //Right of center
    drum1.pitchMod(.5); //have normal pitch mod level
    filter1.resonance(mapf(abs(MotionLevel1 - 1000), 1, 2000, .7, 5));
    sine1.frequency(mapf(abs(MotionLevel1 - 1000), 0, 1023, 0.001, 100)); //set LFO frequency based on motion knob position
} else {
    drum1.pitchMod(mapf(MotionLevel1, 1, 2000, 1, 0)); //bend the pitch down as it plays  
}


//============TEETH KNOB===================
int TeethLevel1 = map(analogRead(pE2), 0, 1023, 1, 2000);

if (TeethLevel1 > 1000) { //Right of center
  bitcrusher1.bits(map(TeethLevel1, 1, 2000, 24, 2));
  bitcrusher1.sampleRate(map(TeethLevel1, 1, 2000, 50200, 1));
  mixer3.gain(3, mapf(abs(TeethLevel1 - 1000), 1, 2000, 0, .5)); 
  mixer3.gain(2,0); //Make sure PWM isn't on
} else { //if left of center...
  bitcrusher1.sampleRate(map(TeethLevel1, 1, 2000, 1, 50200));
  mixer3.gain(2, mapf(abs(TeethLevel1 - 1000), 1, 2000, 0, .2)); 
  mixer3.gain(3, 0); //make sure FM isn't on
}  




//TRIGGER THE DRUM:
if (digitalRead(Trigger1Pin) == HIGH) 
{
    if (playTimes < 1){
      drum1.noteOn();   //Play the drum1 sound just once
      envelope1.noteOn(); //drum envelope
      envelope2.noteOn(); //noise source
//      digitalWrite(Trig1LED, LOW);
    playTimes = playTimes + 1;
    }
    if (playTimes > 1){
      playTimes = playTimes + 1;       //Chill, don't play anything, and increment
//       digitalWrite(Trig1LED, HIGH);
    }
} 
if ((digitalRead(Trigger1Pin) == LOW)){
  playTimes = 0;
//  digitalWrite(Trig1LED, HIGH);
  envelope1.noteOff();
  envelope2.noteOff();
}

//============TROUBLESHOOTING==================
//  Here for troubleshooting when Trigger isn't available
  if (msec >= 500) {
//DRUM1
digitalWrite(1, HIGH);
 // drum1.noteOn();
  //envelope1.noteOn(); //drum envelope
  //envelope2.noteOn(); //noise source
  //delay(100);
  //envelope1.noteOff();
  //envelope2.noteOff();
 // digitalWrite(Trig1LED, HIGH); 
  //delay(500);               // wait for half a second    
  msec = 0;
  } else {
digitalWrite(1, LOW);
  }
    
} //End of main Loop
