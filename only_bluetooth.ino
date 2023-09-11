#include "BluetoothManager.h"

BluetoothManager bt;

bool readyCard = false;
File file;

void openFile(){
  file = SD.open("/output.tar.gz", FILE_WRITE);
  // if(SD.remove("/output.tar.gz")){
  //   Serial.println("borrado");
  // }
}

void initializeSD(){
  if(SD.begin(5)){
    readyCard = true;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  bt.startBluetooth();
  initializeSD();
  openFile();
}

void loop() {
  // put your main code here, to run repeatedly:
  bt.handleFileData(file);
}
