#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;

// Parse the /etc/os-release to find & return the OS release name
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Parse the /proc/version to find & return the kernel identifier
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Parse /proc/meminfo to get mem info to calculate the system memory utilization
// memory utilization (%) = 100 - (((memfree + buffers + cached) * 100) / memtotal)
// I divide by 100 to cancel out the 100 in NCursesDisplay::ProgressBar()
float LinuxParser::MemoryUtilization() {
  string key;
  float memTotal, memFree, buffers, cached;
  float value;
  string line;
  int count = 5;     // We only need to read the first 5 lines
  std::ifstream procMemInfo (kProcDirectory + kMeminfoFilename);
  if (procMemInfo.is_open())
  {
    while (count > 0)
    {
      std::getline(procMemInfo, line);
      count--;
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "MemTotal:")
        {
          memTotal = value;
        }
        else if (key == "MemFree:")
        {
          memFree = value;
        }
        else if (key == "Buffers:")
        {
          buffers = value;
        }
        else if (key == "Cached:")
        {
          cached = value;
        }
      }
    }
  }

return (100 - (((memFree + buffers + cached) * 100) / memTotal)) / 100 ;
}

// Parse /proc/uptime to find and return the system uptime
long LinuxParser::UpTime() {
  long int upTime;
  string line;
  std::ifstream procUpTime (kProcDirectory + kUptimeFilename);
  if (procUpTime.is_open())
  {
    std::getline(procUpTime, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Parse the /proc/stat file to find & return the total number of processes
int LinuxParser::TotalProcesses() {
  string key;
  int value;
  string line;
  std::ifstream procStat(kProcDirectory + kStatFilename);
  if (procStat.is_open())
  {
    while (std::getline(procStat, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes")
        {
          return value;
        }
      }
    }
  }
  return value;
}

// Parse /proc/stat to read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  std::ifstream procStat(kProcDirectory + kStatFilename);
  if (procStat.is_open())
  {
    while (std::getline(procStat,line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "procs_running")
        {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
