#ifndef BLUETOOTH_MANAGER
#define BLUETOOTH_MANAGER

#include "BluetoothSerial.h"
#include <SD.h>
#include <SPI.h>
#include <Ticker.h>
#define COMMAND_LENGTH 5
#define SIZE 515

class BluetoothManager{
  private:
    
    BluetoothSerial SerialBT;
    int lastPacket = 0;
    byte cmdQueue[5] = { 0, 0, 0, 0, 0 };
    int bufferIndex = 0;
    //byte buffer[COMMAND_LENGTH];
    Ticker sender;

    byte dataBuffer[SIZE];
    int byteCount = 0;


    //String actualPath = "/";
    String fileToOperate = "";
    bool fileCreated = false;
    bool passiveMode = false;
    bool downloadMode = false;
    

    void handleCommand(String command);
    
    void retrieveFile();
    int getNumberFromFrame(byte *buffer, int count);
    void processingData(int count, File file);
    bool checkTheFinalPackage(int cnt);
    void writeInformation(int count, File file);

  public:
    bool finishTransfer = false; 
    void serialBTAvailable();
    void startBluetooth();
    void handleFileData(File file);
    static void check();
    static bool sendMsg;
};

#endif