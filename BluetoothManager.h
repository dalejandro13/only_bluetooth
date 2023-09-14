#ifndef BLUETOOTH_MANAGER
#define BLUETOOTH_MANAGER

#include "BluetoothSerial.h"
#include <SD.h>
#include <SPI.h>
#include <Ticker.h>
#include "mp3Player.h"

#define COMMAND_LENGTH 5
#define SIZE 515

class BluetoothManager{
  private:
    const String nameFile = "/record.aac";
    BluetoothSerial SerialBT;
    File file;
    int lastPacket = 0;
    byte cmdQueue[5] = { 0, 0, 0, 0, 0 };
    int bufferIndex = 0;
    //byte buffer[COMMAND_LENGTH];
    Ticker sender, waitForPlay;

    byte dataBuffer[SIZE];
    int byteCount = 0;

    bool isOpen = false;
    //String actualPath = "/";
    String fileToOperate = "";
    bool fileCreated = false;
    bool passiveMode = false;
    bool downloadMode = false;
    
    void handleCommand(String command);
    
    void retrieveFile();
    int getNumberFromFrame(byte *buffer, int count);
    void processingData(int count);
    void writeInformation(int count);
    void closeBluetoothConnection();

  public:
    Mp3Player player = Mp3Player(true);
    bool finishTransfer = false; 
    void serialBTAvailable();
    void startBluetooth();
    void handleFileData();
    static void check();
    static bool sendMsg;
    static void setTrack();
    void openFile();
    void initializeSD();
};

#endif