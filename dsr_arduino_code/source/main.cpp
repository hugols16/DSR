#include <iostream>
#include "DeviceState.cpp"

using namespace std;

int main() {

  DeviceState state;

  if (state.current == INIT) {
    state.transition();
    cout << state.current << endl;
  }
  if (state.current == READY) {
    state.transition();
    cout << state.current << endl;
  }

  // while (1) {

  // }

  return 0;
}
