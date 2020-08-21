#include "processor.h"
#include "linux_parser.h"
#include "iostream"
#include <unistd.h>


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long prevIdle, prevActive, prevTotal;
  long idle, active, total;
  long totald, idled;
  GetJiffies(prevIdle, prevActive, prevTotal);
  sleep (10);
  GetJiffies(idle, active, total);
  totald = total - prevTotal;
  idled = idle - prevIdle;

  return (totald - idled) / totald;

}

void Processor::GetJiffies(long &idle, long &active, long &total)
{
  idle = LinuxParser::IdleJiffies();
  active = LinuxParser::ActiveJiffies();
  total = LinuxParser::Jiffies();
}
