#include <Dezibot.h>
#include <autocharge/Autocharge.hpp>

auto master = MasterData(4200495932);

void step_work(Slave *slave) {
    Serial.printf("Execute 'step_work'\n");
    slave->requestCharge();
}

bool step_to_charge(MasterData &master) {
    Serial.printf("Execute 'step_to_charge' with master %u\n", master.id);
    return true;
}

void step_wait_charge(MasterData &master) {
    Serial.printf("Execute 'step_wait_charge' with master %u\n", master.id);
}

bool step_into_charge(MasterData &master) {
    Serial.printf("Execute 'step_into_charge' with master %u\n", master.id);
    return true;
}

void step_charge(MasterData &master) {
    Serial.printf("Execute 'step_charge' with master %u\n", master.id);
}

bool step_exit_charge(MasterData &master) {
    Serial.printf("Execute 'step_exit_charge' with master %u\n", master.id);
    return true;
}

Slave slave = Slave(SlaveState::WORK, master,
  step_work,
  step_to_charge,
  step_wait_charge,
  step_into_charge,
  step_charge,
  step_exit_charge);

void setup() {
  slave.begin();
}

void loop() {
  delay(1000);
  slave.step();
}
