#include "BluetoothManager.h"
#include "mp3Player.h"

BluetoothManager bt;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  bt.startBluetooth();
  bt.initializeSD();
  //bt.openFile();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!bt.player.notPlaying)
    bt.player.playAudio();

  bt.handleFileData();
}
