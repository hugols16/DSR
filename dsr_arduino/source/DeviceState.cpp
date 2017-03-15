#pragma once

enum STATES {
    INIT,
    READY,
    RAMP_SEARCH,
    RAMP_TURN,
    RAMP_AHEAD,
    RAMP_UP,
    RAMP_LEVEL,
    RAMP_DOWN,
    SEARCHING,
    FOUND_BASE,
    DONE
};

class DeviceState {
public:
  static int current;

  void init() {
    current = INIT;
  }

  void transition () {
    current++;
  }
};

int DeviceState::current;
