#ifndef MASTERDATA_H
#define MASTERDATA_H

#include <cstdint>

struct MasterData {
  uint32_t id;

  MasterData(uint32_t id) : id(id) {}
};

#endif // MASTERDATA_H
