#include "BluetoothManager.h"

bool BluetoothManager::sendMsg = false;

void BluetoothManager::initializeSD(){
  if(SD.begin(5)){
    player.cardExists = true;
  }
  player.initializeOut(26, 25, 27);
  player.setTrackToPlay("/tracks/initial.mp3", 1); //reproducir audio
}

void BluetoothManager::openFile(){
  file = SD.open(nameFile, FILE_WRITE);
  // if(SD.remove("/output.tar.gz")){
  //   Serial.println("borrado");
  // }
}

void BluetoothManager::startBluetooth(){
  SerialBT.begin("ESP32_FTP");
  Serial.write(&"\nEl dispositivo está listo para emparejar\n"[0]);
}

int BluetoothManager::getNumberFromFrame(byte *buffer, int count){
  bool startReading  = false;
  String numberStr = "";
  for (int i = 0; i < count; i++) {
    if (buffer[i] == '/' && buffer[i+1] == 'N' && buffer[i+2] == 'P' && buffer[i+3] == '-'){
      startReading = true;
      i += 4;
    }

    if(startReading){
      if (buffer[i] != '*') {
        numberStr += (char)buffer[i];
      } 
      else {
        break;  // Si encontramos '*', terminamos el ciclo
      }
    }
  }

  if(numberStr.length() > 0){
    lastPacket = atoi(numberStr.c_str());
    return lastPacket;
  }

  return -1;
}

void BluetoothManager::writeInformation(int count){
  for (int i = 0; i < count; i++) {
    file.write(dataBuffer[i]);
    //Serial.print(String((char)dataBuffer[i]));
  }
  SerialBT.print("DONE");
  //file.flush();
}

void BluetoothManager::processingData(int count){
  // if(checkTheFinalPackage(count)){
  //   SerialBT.print("FT-S*");
  //   Serial.println("File transfer stopped");
  //   file.close();
  //   sender.detach();
  //   return;
  // }

  int result = getNumberFromFrame(dataBuffer, count);
  if(result != -1){
    if(lastPacket != 0){
      SerialBT.print("/NP-" + String(lastPacket) + "*");
    }
    else
      SerialBT.print("ERROR1\n");
  }
  else{
    if(lastPacket != 0){
      if(count == lastPacket){
        writeInformation(count);
      }
      else
        SerialBT.print("ERROR2\n");
    }
    else if (count == 512) {
      writeInformation(count);
    }
    else 
      SerialBT.print("ERROR3\n");
  }
  count = 0;
}

void BluetoothManager::check(){
  BluetoothManager::sendMsg = true;
}

void BluetoothManager::closeBluetoothConnection(){
  SerialBT.end();
  startBluetooth();
}

void BluetoothManager::handleFileData() {
  memset(dataBuffer, 0, SIZE); // Limpiar el buffer
  byteCount = 0; // Resetear el contador

  while(SerialBT.available() > 0){
    dataBuffer[byteCount] = SerialBT.read();
    byteCount++;

    if (byteCount >= 5) { // Solo revisar si tenemos al menos 5 bytes en el buffer
      if (dataBuffer[byteCount - 5] == 'F' && dataBuffer[byteCount - 4] == 'T' && dataBuffer[byteCount - 3] == '-' && dataBuffer[byteCount - 2] == 'S' && dataBuffer[byteCount - 1] == '*') {
        SerialBT.print("FT-S*");
        lastPacket = 0;
        file.close();
        isOpen = false;
        sender.detach();
        closeBluetoothConnection();
        player.setTrackToPlay(nameFile, 1); //reproducir audio
        return;
      }
    }
    sender.attach_ms(15000, BluetoothManager::check);
    //delay(3);
  }

  if(BluetoothManager::sendMsg){
    BluetoothManager::sendMsg = false;
    SerialBT.print("ERROR4\n");
    //Serial.println("ERROR1");
    //sender.detach();
  }

  
  // Si ha pasado TIMEOUT milisegundos desde el último byte recibido
  // if((millis() - lastReceivedTimestamp > TIMEOUT)){
  //   if(copyByteCount > 0){
  //     copyByteCount = 0;
  //   }
  // }
  if(byteCount > 0){
    if(!isOpen){
      isOpen = true;
      openFile();
    }
    processingData(byteCount);
  }

  
}