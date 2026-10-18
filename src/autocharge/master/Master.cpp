#include "Master.hpp"
#include "SlaveData.hpp"
#include <Wire.h>

namespace {
constexpr uint8_t BRIDGE_MOTOR_I2C_ADDRESS = 0x12;
constexpr uint8_t BRIDGE_STATUS_READ_SIZE = 32;

enum class BridgeCommand : uint8_t {
  NONE,
  LOWER_TO_WALK_IN,
  LOWER_TO_CHARGE,
  RAISE
};

BridgeCommand bridgeCommandInFlight = BridgeCommand::NONE;
String lastBridgeStatus;

const char *bridgeCommandName(BridgeCommand command) {
  if (command == BridgeCommand::LOWER_TO_WALK_IN) {
    return "LOWER_WALK_IN";
  }
  if (command == BridgeCommand::LOWER_TO_CHARGE) {
    return "LOWER_CHARGE";
  }
  if (command == BridgeCommand::RAISE) {
    return "RAISE";
  }
  return "NONE";
}

const char *bridgeDoneToken(BridgeCommand command) {
  if (command == BridgeCommand::LOWER_TO_WALK_IN) {
    return "DONE:LOWER_WALK_IN";
  }
  if (command == BridgeCommand::LOWER_TO_CHARGE) {
    return "DONE:LOWER_CHARGE";
  }
  if (command == BridgeCommand::RAISE) {
    return "DONE:RAISE";
  }
  return "";
}

bool sendBridgeCommandIfNeeded(BridgeCommand command) {
  if (bridgeCommandInFlight == command) {
    return true;
  }

  if (bridgeCommandInFlight != BridgeCommand::NONE) {
    return false;
  }

  Wire.beginTransmission(BRIDGE_MOTOR_I2C_ADDRESS);
  Wire.print(bridgeCommandName(command));
  Wire.write('\n');
  const uint8_t error = Wire.endTransmission();
  if (error != 0) {
    Serial.printf("Bridge command '%s' failed (i2c error=%u)\n",
                  bridgeCommandName(command), error);
    return false;
  }

  bridgeCommandInFlight = command;
  lastBridgeStatus = "";
  Serial.printf("Bridge command '%s' sent\n", bridgeCommandName(command));
  return true;
}

String readBridgeStatus() {
  String status;
  Wire.requestFrom(BRIDGE_MOTOR_I2C_ADDRESS, BRIDGE_STATUS_READ_SIZE);
  while (Wire.available() > 0) {
    const char ch = static_cast<char>(Wire.read());
    if (ch == '\n' || ch == '\r' || ch == '\0') {
      continue;
    }
    status += ch;
  }
  status.trim();
  return status;
}

bool waitForBridgeCommandDone(BridgeCommand command) {
  if (!sendBridgeCommandIfNeeded(command)) {
    return false;
  }

  const String status = readBridgeStatus();
  if (status.length() == 0) {
    return false;
  }

  if (status != lastBridgeStatus) {
    Serial.printf("Bridge status: %s\n", status.c_str());
    lastBridgeStatus = status;
  }

  if (!status.equals(bridgeDoneToken(command))) {
    return false;
  }

  bridgeCommandInFlight = BridgeCommand::NONE;
  return true;
}
} // namespace

Master *Master::master = nullptr;

void Master::begin(void) {
  this->Dezibot::begin();
  Wire.setTimeOut(50);
  Serial.printf("Bridge I2C ready on shared Wire bus: addr=0x%02X\n",
                BRIDGE_MOTOR_I2C_ADDRESS);
  this->communication.begin();
  this->communication.onReceiveSingle(&onReceiveSingle);
}

void Master::step() {
  switch (this->chargingStationState) {
  case ChargingStationState::OPEN: {
    stepOpen();
    break;
  }
  case ChargingStationState::LOWERING_TO_WALK_IN: {
    const bool lowered = this->stepLowerToWalkIn();
    if (lowered) {
      this->chargingStationState = ChargingStationState::CLOSED;
    }
    break;
  }
  case ChargingStationState::CLOSED:
    this->stepClosed();
    break;
  case ChargingStationState::ATTACHING_GEAR: {
    const bool attached = this->stepAttachGear();
    if (attached) {
      this->chargingStationState = ChargingStationState::SLAVE_CHARGE;
    }
    break;
  }
  case ChargingStationState::SLAVE_CHARGE:
    this->stepSlaveCharge();
    break;
  case ChargingStationState::LIFTING_GEAR: {
    const bool lifted = this->stepLiftGear();
    if (lifted) {
      this->chargingStationState = ChargingStationState::OPEN;
      if (this->currentChargingSlave != nullptr) {
        this->handleSlaveStopChargeRequest(master, this->currentChargingSlave);
      } else {
        Serial.println(
            "Skip stop-charge callback: no active charging slave tracked");
      }
    }
    break;
  }
  }
}

void Master::enjoinCharge(SlaveData *slave) {
  this->communication.unicast(slave->id, "enjoinCharge");
  Serial.printf("Commanded slave(%u) to charge\n", slave->id);
}

void Master::cancelCharge(SlaveData *slave) {
  this->communication.unicast(slave->id, "cancelCharge");
  Serial.printf("Commanded slave(%u) to cancel charge\n", slave->id);
}

void Master::stepOpen() {
  this->multiColorLight.setTopLeds(GREEN);
  if (!this->chargingSlaves.isEmpty()) {
    this->chargingStationState = ChargingStationState::LOWERING_TO_WALK_IN;
  }
}

bool Master::stepLowerToWalkIn() {
  this->multiColorLight.blink(5, YELLOW, TOP, 1000);
  return waitForBridgeCommandDone(BridgeCommand::LOWER_TO_WALK_IN);
}

bool Master::stepAttachGear() {
  this->multiColorLight.blink(5, RED, TOP, 1000);
  return waitForBridgeCommandDone(BridgeCommand::LOWER_TO_CHARGE);
}

bool Master::stepLiftGear() {
  this->multiColorLight.blink(5, GREEN, TOP, 1000);
  return waitForBridgeCommandDone(BridgeCommand::RAISE);
}

void Master::stepClosed() {
  this->multiColorLight.setTopLeds(YELLOW);
  if (this->currentChargingSlave == nullptr &&
      !this->chargingSlaves.isEmpty()) {
    SlaveData *next = this->chargingSlaves.pick();
    if (next != nullptr) {
      this->currentChargingSlave = next;
      this->enjoinCharge(next);
    }
  }
  delay(5000);
}

void Master::stepSlaveCharge() {
  this->multiColorLight.setTopLeds(RED);
  delay(5000);
}

void Master::handleWorkInfo(SlaveData *slave) {
  slave->state = SlaveState::WORK;
}

void Master::handleWalkToChargeInfo(SlaveData *slave) {
  slave->state = SlaveState::WALKING_TO_CHARGE;
}

void Master::handleInWaitInfo(SlaveData *slave) {
  slave->state = SlaveState::WAIT_CHARGE;
  this->chargingSlaves.insert(slave);
}

void Master::handleWalkIntoChargeInfo(SlaveData *slave) {
  slave->state = SlaveState::WALKING_INTO_CHARGE;
}

void Master::handleInChargeInfo(SlaveData *slave) {
  if (this->currentChargingSlave == nullptr) {
    Serial.printf("Ignoring notifyInCharge from Node(%u): no active slave\n",
                  slave->id);
    return;
  }
  if (this->currentChargingSlave->id != slave->id) {
    Serial.printf("Ignoring notifyInCharge from Node(%u): active charging "
                  "slave is Node(%u)\n",
                  slave->id, this->currentChargingSlave->id);
    return;
  }
  slave->state = SlaveState::CHARGE;
  this->chargingStationState = ChargingStationState::ATTACHING_GEAR;
}

void Master::handleExitChargeInfo(SlaveData *slave) {
  slave->state = SlaveState::EXITING_CHARGE;
  this->currentChargingSlave = nullptr;
  this->chargingStationState = ChargingStationState::OPEN;
}
