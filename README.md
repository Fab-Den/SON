# SON

## Making an Autotune
Our choice for the 'projet son' was to try replicating an auto-tune using the Teensy. We use a function from the Teensy audio library to detect the input frequency and then a Faust program to transpose it to the nearest known frequency of a musical note. The principle of auto-tuning and the working of the different functions we use are detailed on the project poster, as well as an history of our attempt at implementing an FFT on the Teensy.  
 
To replicate the specific sound usually associated with an 'auto-tune', a digital interface allows the user to toggle different parameters. One can choose to disable the smoothing of the transitions between the transposed frequencies so that the output seems more robotic, add and control a reverberation effect (from the teensy audio library), and add a replication of the input sound transposed to lower frequencies to create a major or minor triad chord. 

## Using the Auto-tune on the Teensy
After uploading the programme_faust.ino program on the Teensy (nested in the programme_faust folder), you will be able to test the default output of our auto-tune using a microphone and earphones. The default output is with smoothing on, reverb off, and chords off (so the output should be the just one transposition of the input sound). 
To use the GUI in order to change these parameters you need to install blablablablabla.

## Code description
The root of this repository contains :
- *programme_faust* the arduino sketch folder that includes the arduino code to upload on the teensy and the library compiled from the Faust Program (named Shifter) 
  program description
- *Shifter.dsp* the faust code we use in the project 
  program description
- a pdf of our presentation poster
