#ifndef SLAVEDATA_H
#define SLAVEDATA_H

#include "../slave/SlaveState.hpp"
#include <cstdint>

class SlaveData {
public:
  const uint32_t id;
  SlaveState state;

  SlaveData(uint32_t id) : id(id), state(SlaveState::WORK) {}
  SlaveData(uint32_t id, SlaveState state) : id(id), state(state) {}
};

#endif // SLAVEDATA_H
