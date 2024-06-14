#include "Communication.h"

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
uint32_t Communication::groupNumber = 0;

// User-defined callback function pointer
void (*Communication::userCallback)(String &msg) = nullptr;

void Communication::sendMessage(String msg)
{
    String data = String(groupNumber) + "#" + msg;
    mesh.sendBroadcast(data);
}

// Needed for painless library
void Communication::receivedCallback(uint32_t from, String &msg)
{
    int separatorIndex = msg.indexOf('#');
    if (separatorIndex != -1) {
        String groupNumberStr = msg.substring(0, separatorIndex);
        uint32_t num = groupNumberStr.toInt();
        String restOfMsg = msg.substring(separatorIndex + 1);

        Serial.printf("startHere: Received from %u groupNumber=%u msg=%s\n", from, num, restOfMsg.c_str());

        if (groupNumber != num) return;

        // Execute user-defined callback if it is set
        if (userCallback) {
            userCallback(restOfMsg);
        }
    }
}

void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void vTaskUpdate(void *pvParameters)
{
    for (;;)
    {
        mesh.update();
    }
}

void Communication::setGroupNumber(uint32_t number) {
    groupNumber = number;
}

// Method to set the user-defined callback function
void Communication::onReceive(void (*callbackFunc)(String &msg))
{
    userCallback = callbackFunc;
}

void Communication::begin(void)
{
    Serial.begin(115200);

    // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages

    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    xTaskCreate(vTaskUpdate, "vTaskMeshUpdate", 4096, &ucParameterToPass, tskIDLE_PRIORITY, &xHandle);
    configASSERT(xHandle);
};