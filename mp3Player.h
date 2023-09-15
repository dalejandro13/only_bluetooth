#ifndef MP3_PLAYER
#define MP3_PLAYER

#include <AudioFileSourceSD.h>
//#include <AudioOutputI2SNoDAC.h>
#include <AudioOutputI2S.h>

#include <AudioGeneratorMP3.h>
//#include "AudioGeneratorAAC.h"
//#include "AudioGeneratorFLAC.h"

#define DEFAULT_SAMPLE_RATE 44100
#define DEFAULT_BITS 16;
#define DEFAULT_CHANNELS 2;

class Mp3Player
{
  private:
      String printFormat = "[MP3]: ";
      AudioFileSourceSD *file;
      AudioGeneratorMP3 *format;
      //AudioGeneratorAAC *format;
      //AudioGeneratorFLAC *flac;

      //AudioOutputI2SNoDAC *output;
      AudioOutputI2S *output;
      String actualTrack = "";
      int playItarations = 1;
      
      
      bool debug = false;
      void initializeMp3();
      void initializeTrack();
      void initializeAudioFile();

  public:
      
      
      void debugPrint(String message);
      bool cardExists = false, notPlaying = true;
      
      Mp3Player(){};
      Mp3Player(bool debug)
      {
        this->debug = debug;
      }
      void initializeOut(int bclkPin, int wclkPin, int doutPin);
      void playAudio();
      void stop();
      void volumeControl(float value);
      void setTrackToPlay(String trackName, int iterations);
};

#endif