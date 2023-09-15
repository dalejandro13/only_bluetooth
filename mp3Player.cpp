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
  format = new AudioGeneratorMP3();
  //format = new AudioGeneratorFLAC();
  //format = new AudioGeneratorAAC();
  debugPrint("AudioGeneratorMP3 initialized");
  if (format->begin(file, output)){
    notPlaying = false;
    debugPrint("Play started");
  }
  else
    debugPrint("Play not started");
}

void Mp3Player::initializeAudioFile()
{
  debugPrint("Reading file from SD: " + actualTrack + "\n");
  String filePath = actualTrack;
  file = new AudioFileSourceSD(filePath.c_str());
  // if(file->isOpen()){
  //   int size = file->getSize();
  //   Serial.print("tamaño: ");
  //   Serial.println(String(size));
  // }
}

void Mp3Player::stop()
{
  if (file)
  {
    format->stop();
    delete format;
    delete file;
    file = nullptr;
    format = nullptr;
    debugPrint("Resources released");
  }
}

void Mp3Player::playAudio()
{
  if (cardExists && format != NULL && file != NULL)
  {
    if (format->isRunning())
    {
      if (!format->loop())
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
    notPlaying = false;
  }
  else
  {
    debugPrint("SDCard do not exists");
  }
}

void Mp3Player::setTrackToPlay(String trackName, int iterations){
  //notPlaying = false;
  actualTrack = trackName;
  playItarations = iterations;
  initializeTrack();
}