#pragma once

#include "../../Dezibot.h"
#include "MasterData.hpp"
#include "SlaveState.hpp"
#include <functional>

class Slave : public Dezibot {
public:
  Slave(SlaveState state, MasterData &master,
        const std::function<void(Slave *slave)> stepWork,
        const std::function<bool(Slave *slave, MasterData &master)> stepToCharge,
        const std::function<void(Slave *slave, MasterData &master)> stepWaitCharge,
        const std::function<bool(Slave *slave, MasterData &master)> stepIntoCharge,
        const std::function<void(Slave *slave, MasterData &master)> stepCharge,
        const std::function<bool(Slave *slave, MasterData &master)> stepExitCharge)
      : state(state), master(master), stepWork(stepWork),
        stepToCharge(stepToCharge), stepWaitCharge(stepWaitCharge),
        stepIntoCharge(stepIntoCharge), stepCharge(stepCharge),
        stepExitCharge(stepExitCharge) {
    slave = this;
  }

  static Slave *slave;

  void begin() override;
  void step();
  void requestCharge();
  void requestStopCharge();

private:
  SlaveState state;
  MasterData &master;
  const std::function<void(Slave *slave)> stepWork;
  const std::function<bool(Slave *slave, MasterData &master)> stepToCharge;
  const std::function<void(Slave *slave, MasterData &master)> stepWaitCharge;
  const std::function<bool(Slave *slave, MasterData &master)> stepIntoCharge;
  const std::function<void(Slave *slave, MasterData &master)> stepCharge;
  const std::function<bool(Slave *slave, MasterData &master)> stepExitCharge;

  void notifyWork();
  void notifyWalkToCharge();
  void notifyInWait();
  void notifyWalkIntoCharge();
  void notifyInCharge();
  void notifyExitCharge();

  void handleEnjoinChargeCommand();
  void handleCancelChargeCommand();

  static void onReceiveSingle(uint32_t from, String &message) {
      Serial.printf("Received single from Node(%u): %s\n", from, message.c_str());
    if (from == slave->master.id) {
      if (message.equals("enjoinCharge")) {
        slave->handleEnjoinChargeCommand();
      } else if (message.equals("cancelCharge")) {
        slave->handleCancelChargeCommand();
      }
    }
  }
};
