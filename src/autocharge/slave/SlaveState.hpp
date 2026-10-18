#pragma once

enum class SlaveState {
  WORK,
  WALKING_TO_CHARGE,
  WAIT_CHARGE,
  WALKING_INTO_CHARGE,
  CHARGE,
  EXITING_CHARGE
};
