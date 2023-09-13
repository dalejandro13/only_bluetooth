#include "mp3Player.h"

void Mp3Player::debugPrint(String message)
{
  if (debug)
  {
    String msg = "\n" + printFormat + message;
    Serial.write(&msg[0]);
    yield();
  }
}

void Mp3Player::initializeOut(int bclkPin, int wclkPin, int doutPin)
{
  debugPrint("InitializeOutput Method");
  // output = new AudioOutputI2SNoDAC();
  output = new AudioOutputI2S();
  output->SetPinout(bclkPin, wclkPin, doutPin);
  debugPrint("Pinout (bclk, wclk, dout): " + String(bclkPin) + ", " + String(wclkPin) + ", " + String(doutPin));
  output->SetBitsPerSample(16);
  output->SetChannels(2);
  output->SetRate(DEFAULT_SAMPLE_RATE);
  volumeControl(1.0);
}

void Mp3Player::volumeControl(float value){
  output->SetGain(value);  //1.0: normal volume
}

void Mp3Player::initializeMp3()
{
  mp3 = new AudioGeneratorMP3();
  debugPrint("AudioGeneratorMP3 initialized");
  if (mp3->begin(file, output)){
    notPlaying = false;
    debugPrint("Play started");
  }
  else
    debugPrint("Play not started");
}

void Mp3Player::initializeAudioFile()
{
  debugPrint("Reading file from SD: " + actualTrack);
  String filePath = actualTrack;
  file = new AudioFileSourceSD(filePath.c_str());
}

void Mp3Player::stop()
{
  if (file)
  {
    mp3->stop();
    delete mp3;
    delete file;
    file = nullptr;
    mp3 = nullptr;
    debugPrint("Resources released");
  }
}

void Mp3Player::playAudio()
{
  if (cardExists && mp3 != NULL && file != NULL && ready)
  {
    if (mp3->isRunning())
    {
      if (!mp3->loop())
      {
        debugPrint("MP3 Resource Finished");
        stop();
        playItarations--;
        if(playItarations <= 0){
          debugPrint("MP3 Repetition Loop Ended");
          notPlaying = true;
        }
        else
          initializeTrack();
      }
    }
    else
    {
      debugPrint("MP3 Not Running");
      notPlaying = true;
    }
  }
}

void Mp3Player::initializeTrack()
{
  if (cardExists && actualTrack != "")
  {
    initializeAudioFile();
    initializeMp3();
  }
  else
  {
    debugPrint("SDCard do not exists");
  }
}

void Mp3Player::setTrackToPlay(String trackName, int iterations){
  notPlaying = false;
  actualTrack = trackName;
  playItarations = iterations;
  initializeTrack();
}