
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

    void broadcast(String msg);

    void unicast(uint32_t targetId, String msg);

    void onReceiveGroup(void (*callbackFunc)(uint32_t from, String &msg));

    void onReceiveSingle(void (*callbackFunc)(uint32_t from, String &msg));

    uint32_t getNodeId();

private:
    static void (*userCallbackSingle)(uint32_t from, String &msg);
    static void (*userCallbackGroup)(uint32_t from, String &msg);
    static void receivedCallback(uint32_t from, String &msg);
    static uint32_t groupNumber;

};
#endif //Communication_h
