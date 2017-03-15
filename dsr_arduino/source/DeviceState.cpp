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
  int current;

  DeviceState() {
    current = INIT;
  }

  void transition () {
    current++;
  }
};
