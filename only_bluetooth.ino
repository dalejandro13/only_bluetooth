#include "BluetoothManager.h"
#include "mp3Player.h"

//BluetoothSink bluetoothSink;
BluetoothManager bt;
String data = String();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  bt.startBluetooth();
  bt.initializeSD();
  //bt.openFile();
}

void serialCommunication(){
  while(Serial.available() > 0){
    char c = Serial.read();
    data += c;
    delay(3);
  }
  if(data != ""){
    if(data.compareTo("play") == 0)
      bt.player.setTrackToPlay("/tracks/initial.aac", 1);
    
    data = String();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  serialCommunication();

  if(!bt.player.notPlaying)
    bt.player.playAudio();

  bt.handleFileData();
}
