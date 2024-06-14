
#ifndef Communication_h
#define Communication_h
#include <stdint.h>
#include <Arduino.h>
#include <painlessMesh.h>

#define   MESH_PREFIX     "DEZIBOT_MESH"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555


class Communication{
public: 
    /**
     * @brief initialize the Mesh Compnent, must be called before the other methods are used.  
     * 
     */
    static void begin(void);

    void setGroupNumber(uint32_t number);

    void sendMessage(String msg);

    void onReceive(void (*callbackFunc)(String &msg));
private:
    static void (*userCallback)(String &msg);
    static void receivedCallback(uint32_t from, String &msg);
    static uint32_t groupNumber;

};
#endif //Communication_h