#include "storageManager.h"

bool StorageManager::begin()
{
    return SPIFFS.begin(true);
}

bool StorageManager::exists(char *path)
{
    return SPIFFS.exists(path);
}

bool StorageManager::exists(String path)
{
    return this->exists(&path[0]);
}

bool StorageManager::appendFile(char *path, char *message)
{
    file = SPIFFS.open(path, "a");
    if (!file)
    {
        return false;
    }
    file.print(message);
    file.close();
    return true;
}

bool StorageManager::writeFile(char *path, char *message)
{
    file = SPIFFS.open(path, "w");
    if (!file)
    {
        return false;
    }
    file.print(message);
    file.close();
    return true;
}

bool StorageManager::removeFile(char *path)
{
    return SPIFFS.remove(path);
}

uint8_t StorageManager::getTotalCommMessages()
{
    uint8_t messagesQuantity = 0;
    if (this->exists(COMMUNICATION_CHANNEL_PATH))
    {
        file = SPIFFS.open(COMMUNICATION_CHANNEL_PATH, "r");
        if (!file)
            return 0;
        while (file.available())
        {
            file.readStringUntil('\n');
            messagesQuantity++;
        }
        file.close();
    }
    return messagesQuantity;
}

bool StorageManager::addMessageToCommChannel(String message)
{
    return this->appendFile(COMMUNICATION_CHANNEL_PATH, message);
}

bool StorageManager::cleanCommChannelFile()
{
    return this->removeFile(COMMUNICATION_CHANNEL_PATH);
}

String StorageManager::getMessageFromIndex(uint8_t index)
{
    String message = "";
    uint8_t actualIndex = 0;
    if (this->exists(COMMUNICATION_CHANNEL_PATH))
    {
        file = SPIFFS.open(COMMUNICATION_CHANNEL_PATH, "r");
        if (!file)
            return "";
        while (file.available())
        {
            if (actualIndex == index)
            {
                message = file.readStringUntil('\n');
                file.close();
                return message;
            }
            file.readStringUntil('\n');
            actualIndex++;
        }
        file.close();
    }
    return message;
}

bool StorageManager::commMessageExists(String message)
{
    bool messageExists = false;
    String mess = "";
    if (this->exists(COMMUNICATION_CHANNEL_PATH))
    {
        file = SPIFFS.open(COMMUNICATION_CHANNEL_PATH, "r");
        if (!file)
            return false;
        while (file.available())
        {
            mess = file.readStringUntil('\n');
            mess.replace("\n", "");
            if (message.compareTo(mess) >= 0)
                messageExists = true;
        }
        file.close();
    }
    return messageExists;
}

uint8_t StorageManager::getBrightness()
{
    if (this->exists(BRIGHTNESS_FILE_PATH))
    {
        file = SPIFFS.open(BRIGHTNESS_FILE_PATH, "r");
        if (!file)
            return 125;

        String data = file.readStringUntil('\n');
        file.close();

        int readedBrightness = data.toInt();

        if (readedBrightness >= 0 && readedBrightness <= 255)
            return (uint8_t)readedBrightness;
    }
    else
    {
        this->writeFile(BRIGHTNESS_FILE_PATH, String(125) + "\n");
    }
    return 125;
}

void StorageManager::setNewBrightness(uint8_t value){
    this->writeFile(BRIGHTNESS_FILE_PATH, String(value) + "\n");
    Serial.println("Nuevo brillo guardado: " + String(value));
}