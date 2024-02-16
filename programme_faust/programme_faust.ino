#include <Audio.h>
#include "Shifter.h"
#include "math.h"
#include <cmath>

AudioOutputI2S out;
AudioInputI2S in;
Shifter shifter;
AudioAnalyzeNoteFrequency notefreq;
AudioControlSGTL5000 audioShield;
// --------
AudioConnection patchCord4(in, 0, notefreq, 0);
AudioConnection patchCord1(in, 0, shifter,0);
AudioConnection patchCord2(shifter,0,out,0);
AudioConnection patchCord3(shifter,0,out,1);
float notes[] = {65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98.0, 103.83, 110.0, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.0, 196.0, 207.65, 220.0, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.0, 415.3, 440.0, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880.0, 932.33, 987.77, 1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98,1661.22,1760.0, 1864.66, 1975.53};

float searchFreq(float note, float freqs[60]) {
    float closest = freqs[0];
    float min_diff = abs(note - freqs[0]);

    for (int i = 0; i<60; i++) {
        float diff = abs(freqs[i]-note);
        if (diff<min_diff) {
            min_diff = diff;
            closest = freqs[i];
        }
        if (min_diff == 0) {
            break;
        }
    }
    return closest;
}


void setup() {
  AudioMemory(30);
  audioShield.enable();
  audioShield.volume(0.5);
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(10);
  Serial.begin(9600);
  notefreq.begin(.15);
  shifter.setParamValue("gain", 1);
  shifter.setParamValue("xfade", 10); //de 10 à 1000
  shifter.setParamValue("window", 1000); //de 10 à 10 000
  shifter.setParamValue("mode",2); // 1 pour neutre, 2 pour accord mineur, 3 pour majeur
  // le shift est aussi paramétrable en dur de -20 à 20 demi-tons
  }

void loop() {
  if (notefreq.available()) {
    
        float note = notefreq.read();
        float prob = notefreq.probability(); 
       
        float freq = searchFreq(note,notes);
        
        Serial.printf("Note: %3.2f | Probability: %.2f | Ref: %3.2f\n", note, prob,freq);
        
        float ref = 1.0/12.0;        
        float shift = (log(freq/note)/log(2))/ref;
        Serial.println(shift);
        shifter.setParamValue("shift",shift);
  
}}
