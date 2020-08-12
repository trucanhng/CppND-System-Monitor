#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Format the times (in seconds) to HH:MM:SS form
string Format::ElapsedTime(long times) {
  int hours;
  int minutes;
  int seconds;
  long remainder;
  hours = times / 3600;
  // One way to calculate minutes and seconds
  remainder = times - (hours * 3600);
  minutes = remainder / 60;
  seconds = remainder - (minutes * 60);
  // Another way to calculate minutes and seconds
  //minutes = (times % 3600) / 60;
  //seconds = times % 60;
  return to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
}
