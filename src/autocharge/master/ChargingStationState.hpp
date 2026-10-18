#pragma once

enum class ChargingStationState {
  OPEN,
  LOWERING_TO_WALK_IN,
  CLOSED,
  ATTACHING_GEAR,
  SLAVE_CHARGE,
  LIFTING_GEAR,
};
