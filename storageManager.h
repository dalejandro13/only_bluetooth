#ifndef STORAGE_MANAGER
#define STORAGE_MANAGER

#include <FS.h>
#include <SPIFFS.h>

#define COMMUNICATION_CHANNEL_PATH "/communications.channel"
#define BRIGHTNESS_FILE_PATH "/brightness.fls"

class StorageManager
{

private:
    File file;

    
    bool exists(char *path);
    bool exists(String path);
    bool appendFile(char *path, char *message);
    bool appendFile(String path, char *message)
    {
        return this->appendFile(&path[0], message);
    }
    bool appendFile(String path, String message)
    {
        return this->appendFile(&path[0], &message[0]);
    }

    bool writeFile(char *path, char *message);
    bool writeFile(String path, char *message)
    {
        return this->writeFile(&path[0], message);
    }
    bool writeFile(String path, String message)
    {
        return this->writeFile(&path[0], &message[0]);
    }

    bool removeFile(char* path);
    bool removeFile(String path){
        return this->removeFile(&path[0]);
    }


public:
    StorageManager() {}
    bool begin();
    uint8_t getTotalCommMessages();
    String getMessageFromIndex(uint8_t index);
    bool addMessageToCommChannel(String message);
    bool cleanCommChannelFile();
    bool commMessageExists(String message);
    uint8_t getBrightness();
    void setNewBrightness(uint8_t value);
};

#endif
