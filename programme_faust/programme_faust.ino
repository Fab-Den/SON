#include <Audio.h>
#include "Shifter.h"
#include "math.h"


AudioOutputI2S out;
AudioInputI2S in;
Shifter shifter;
AudioAnalyzeNoteFrequency notefreq;
AudioControlSGTL5000 audioShield;
AudioFilterBiquad biquad;
// --------
AudioConnection patchCord0(in, 0, biquad, 0);
AudioConnection patchCord4(biquad, 0, notefreq, 0);
AudioConnection patchCord1(in, 0, shifter,0);
AudioConnection patchCord2(shifter,0,out,0);
AudioConnection patchCord3(shifter,0,out,1);
float notes[] = {55.0, 58.27, 61.74, 65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98.0, 103.83, 110.0, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.0, 196.0, 207.65, 220.0, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.0, 415.3, 440.0, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880.0, 932.33, 987.77, 1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760.0, 1864.66, 1975.53, 2093.0, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.0, 3729.31, 3951.07, 4186.01, 4434.92, 4698.64, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88};

void setup() {
  AudioMemory(30);
  audioShield.enable();
  audioShield.volume(0.5);
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  biquad.setHighpass(0, 100);
  audioShield.micGain(10);
  Serial.begin(9600);
  notefreq.begin(.15);
  // shifter.setParamValue("freq",350);
  shifter.setParamValue("gain", 1);
  }

void loop() {
  float freq = 200.0;
  Serial.println("Loop");
  delay(100);
  if (notefreq.available()) {
        float note = notefreq.read();
        float prob = notefreq.probability(); 
        Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
        // float power = 1.0/12.0;
        
        float ref = 1.0/12.0;
        Serial.println(ref);
        
        float shift = (log(freq/note)/log(2))/ref;
        Serial.println(shift);
        shifter.setParamValue("shift",shift);
    }
  
}
