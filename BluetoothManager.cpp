#include "BluetoothManager.h"

bool BluetoothManager::sendMsg = false;

void BluetoothManager::startBluetooth(){
  SerialBT.begin("ESP32_FTP");
  Serial.write(&"El dispositivo está listo para emparejar"[0]);
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

bool BluetoothManager::checkTheFinalPackage(int cnt){
  if(cnt > 0){
    for(int i = 0; i < cnt; i++){
      if(dataBuffer[i] == 'F' && dataBuffer[i+1] == 'T' && dataBuffer[i+2] == '-' && dataBuffer[i+3] == 'S' && dataBuffer[i+4] == '*'){
        lastPacket = 0;
        return true;
      }
    }
  }
  return false;
}

void BluetoothManager::writeInformation(int count){
  for (int i = 0; i < count; i++) {
    file.write(dataBuffer[i]);
    //Serial.print(String((char)dataBuffer[i]));
  }
  SerialBT.print("DONE");
  //file.flush();
}

void BluetoothManager::processingData(int count, File file){
  // if(checkTheFinalPackage(count)){
  //   SerialBT.print("FT-S*");
  //   Serial.println("File transfer stopped");
  //   //file.close();
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

void BluetoothManager::handleFileData(File file) {
  memset(dataBuffer, 0, SIZE); // Limpiar el buffer
  byteCount = 0; // Resetear el contador

  while(SerialBT.available() > 0){
    dataBuffer[byteCount] = SerialBT.read();
    byteCount++;

    if (byteCount >= 5) { // Solo revisar si tenemos al menos 5 bytes en el buffer
      if (dataBuffer[byteCount - 5] == 'F' && dataBuffer[byteCount - 4] == 'T' && dataBuffer[byteCount - 3] == '-' && dataBuffer[byteCount - 2] == 'S' && dataBuffer[byteCount - 1] == '*') {
        SerialBT.print("FT-S*");
        //Serial.println("File transfer stopped");
        lastPacket = 0;
        file.close();
        sender.detach();
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
    processingData(byteCount, file);
  }

  
}