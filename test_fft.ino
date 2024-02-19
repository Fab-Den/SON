#include <Audio.h>
//#include "customFFT.h"



#ifndef uanalyze_fft256_h_
#define uanalyze_fft256_h_

#include <Arduino.h>     // github.com/PaulStoffregen/cores/blob/master/teensy4/Arduino.h
#include <AudioStream.h> // github.com/PaulStoffregen/cores/blob/master/teensy4/AudioStream.h
#include <arm_math.h>    // github.com/PaulStoffregen/cores/blob/master/teensy4/arm_math.h
#include "arm_math.h"

#define FFTlength 1024  // Use 256 or 1024 (must be radix-4)

class AudioProcess : public AudioStream
{
public:
  AudioProcess() : AudioStream(1, inputQueueArray){

    
    
  arm_cfft_radix4_init_q15(&forwardFFT, FFTlength, 0, 1);     // Initialize forward FFT
  arm_cfft_radix4_init_q15(&inverseFFT, FFTlength, 1, 1);     // Initialize inverse FFT

  state = 0;


  }
  
  virtual void update(void);

private:

  audio_block_t* audioBlocks[8]; 
  audio_block_t* outAudioBlocks[4]; 

  int state;

  

  
  int16_t buffer[FFTlength*2] __attribute__ ((aligned (4)));

  audio_block_t *inputQueueArray[1];

  int counter = 0;
  int c = 0;

  
  arm_cfft_radix4_instance_q15 forwardFFT;
  arm_cfft_radix4_instance_q15 inverseFFT; //////////////////////////////////////////////////////////
  
};

#endif




static void copy_to_fft_buffer(void *destination, const void *source)
{
  
  const uint16_t *src = (const uint16_t *)source;
  uint32_t *dst = (uint32_t *)destination;
  
  for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {

    *dst++ = *src++;  // real sample plus a zero for imaginary
  }

}

static void copy_from_fft_buffer(void *destination, const void *source)
{
  const uint32_t *src = (const uint32_t *)source;
  uint16_t *dst = (uint16_t *)destination;

  for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    *dst++ = (uint16_t)(*src++ & 0xFFFF);  // Extrayez les 16 bits de poids faible
  }
  
}


void AudioProcess::update(void)
{
  Serial.println("Coucou");
  audio_block_t *block;

  block = receiveReadOnly();
  if (!block) return;
/*
  for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
    if (c<40){
              block->data[i] = 5000;

    } else {
          block->data[i] = -5000;
    }

    ++c;
    if (c>=80){
      c=0;
    }   
  }*/
  
  switch (state){
    case 0:
      audioBlocks[0] = block;
      state=1;
      break;
    case 1:
      audioBlocks[1] = block;
      state=2;
      break;
    case 2:
      audioBlocks[2] = block;
      state=3;
      break;
    case 3:
      audioBlocks[3] = block;
      state=4;
      break;
    case 4:
      audioBlocks[4] = block;
      if (outAudioBlocks[1]){transmit(outAudioBlocks[1]); release(outAudioBlocks[1]);}
      state=5;
      break;
    case 5:
      audioBlocks[5] = block;
      if (outAudioBlocks[2]){transmit(outAudioBlocks[2]); release(outAudioBlocks[2]);}
      state=6;
      break;
    case 6:
      audioBlocks[6] = block;
      if (outAudioBlocks[3]){transmit(outAudioBlocks[3]); release(outAudioBlocks[3]);Serial.println("PRINT 3");}
      state=7;
      break;
    case 7:
    Serial.println("ici");
      audioBlocks[7] = block;
      
      for (int i=0; i<8; ++i){ 
       /* for (int j=0; j<128; ++j){
          audioBlocks[i]->data[j] *= 64;
        }*/
          copy_to_fft_buffer(buffer+256*i, audioBlocks[i]->data); }

      arm_cfft_radix4_q15(&forwardFFT, buffer);             // Forward FFT
      //for (int i=0; i<2048; ++i){buffer[i] = buffer[i]; Serial.print(buffer[i]);Serial.print(" ");}Serial.println();
      for (int i=0; i<2048; ++i){buffer[i] = buffer[i]; Serial.print(buffer[i]);Serial.print(" ");}Serial.println();

      //for (int i=0; i<2048; ++i){if (i>=1024 && i<1024){buffer[i] = 0;};}
      //for (int i=0; i<2048; ++i){buffer[i]*=64;}


      arm_cfft_radix4_q15(&inverseFFT, buffer);             // inverse FFT
      for (int i=0; i<2048; ++i){buffer[i] = buffer[i]*16;}
      
      float alpha = 0.1;
      for (int i=2; i<2048; i += 2){buffer[i] = buffer[i-2] + alpha*(buffer[i]-buffer[i-2]);}Serial.println();


      outAudioBlocks[0] = allocate();
      outAudioBlocks[1] = allocate();
      outAudioBlocks[2] = allocate();
      outAudioBlocks[3] = allocate();

      copy_from_fft_buffer(outAudioBlocks[0]->data, buffer);
      copy_from_fft_buffer(outAudioBlocks[1]->data, buffer+512);
      copy_from_fft_buffer(outAudioBlocks[2]->data, buffer+1024);
      copy_from_fft_buffer(outAudioBlocks[3]->data, buffer+1536);

      audioBlocks[0] = audioBlocks[4];
      audioBlocks[1] = audioBlocks[5];
      audioBlocks[2] = audioBlocks[6];
      audioBlocks[3] = audioBlocks[7];

      release(audioBlocks[4]);
      release(audioBlocks[5]);
      release(audioBlocks[6]);
      release(audioBlocks[7]);
      
      if (outAudioBlocks[0]){transmit(outAudioBlocks[0]); release(outAudioBlocks[0]);}

      state=4;
      break;
     
  }
/*
  if (audioBlocks[0]){
  release(audioBlocks[0]);
  }
  
  for (int i=0; i<7; ++i){
    audioBlocks[i] = audioBlocks[i+1];
  }
  audioBlocks[7] = block;
  
  
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        if (c<50){
                  block->data[i] = 5000;

        } else {
              block->data[i] = -5000;
        }

        ++c;
        if (c>=100){
          c=0;
        }   
    }


  if (!audioBlocks[0]) return;
  
  Serial.println("Block In : "); for (int i = 0; i<128; ++i){Serial.print(block->data[i]);Serial.print(" ");} Serial.println("");

  if (counter == 4){counter = 0;}
  
  if (counter == 0){
          
          for (int i=0; i<8; ++i){
              copy_to_fft_buffer(buffer+256*i, audioBlocks[i]->data);
          }
          
            Serial.println("Buffer In : "); for (int i = 0; i<2048; ++i){Serial.print(buffer[i]);Serial.print(" ");} Serial.println("");

           arm_cfft_radix4_q15(&forwardFFT, buffer);             // Forward FFT
           
           for (int i = 0; i < 2*FFTlength; i++) {
                if (i<0 || buffer[i]<1){
                  buffer[i] = 0;
                } else {
                  buffer[i] = buffer[i]*128;
        
                }
            }
        
        //  Serial.println("Buffer interm&diaire: "); for (int i = 0; i<512; ++i){Serial.print(buffer[i]);Serial.print(" ");} Serial.println("");
        
          arm_cfft_radix4_q15(&inverseFFT, buffer);             // inverse FFT
        
           for (int i = 0; i < 2*FFTlength; i++) {
                buffer[i] = buffer[i]*32;
            }
           Serial.println("Buffer Out : "); for (int i = 0; i<2048; ++i){Serial.print(buffer[i]);Serial.print(" ");} Serial.println("");

        int u=0;
        //copy_from_fft_buffer(outBlock1->data, buffer+(256*counter));
        for (int j=0; j<4; ++j){
          for (int i=0; i<128; ++i){
            outBlocks[j]->data[i] = buffer[2*u];
            ++u;
          }
      
        }
  
  }*/

  /*
  audio_block_t *outBlock1 = allocate();
  
  Serial.println("Block out : "); for (int i = 0; i<128; ++i){Serial.print(outBlock1->data[i]);Serial.print(" ");} Serial.println("");

  transmit(outBlocks[counter], 0);
  release(outBlocks[counter]);

  ++counter;*/

}









AudioProcess customProcessor;

AudioInputI2S audioInput;
AudioOutputI2S out; // Sortie casque gauche

AudioConnection patchCordIn(audioInput, customProcessor); // Connecter l'entrée audio au processeur audio personnalisé

AudioConnection patchCordLeft(customProcessor, 0, out, 0); // Connecter la sortie casque gauche
AudioConnection patchCordRight(customProcessor, 0, out, 1); // Connecter la sortie casque droit


AudioControlSGTL5000 audioShield;

void setup() {
  
    AudioMemory(20); // Allouer de la mémoire audio
    audioShield.enable();
    audioShield.inputSelect(AUDIO_INPUT_MIC);
    audioShield.micGain(50);
    audioShield.volume(1);
    
}

void loop() {
    // Votre code de traitement audio peut aller ici
}
