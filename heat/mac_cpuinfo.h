#pragma once

#include "smctemp.h"

#include <glog/logging.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

namespace heat {

inline std::string exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

class CpuInfo {
 public:
  float GetCpuTemp() {
    smctemp::SmcTemp smc_temp = smctemp::SmcTemp();
    return smc_temp.GetCpuTemp();
  }

  float GetGpuTemp() {
    smctemp::SmcTemp smc_temp = smctemp::SmcTemp();
    return smc_temp.GetGpuTemp();
  }

  float GetAdapterPowerWatt() {
    auto watts = exec(
        "ioreg -rw0 -c AppleSmartBattery"
        " | grep BatteryData"
        " | grep -o '\"AdapterPower\"=[0-9]*'"
        " | cut -c 16-"
        " | xargs -I % lldb --batch -o \" print/f %\""
        " | tail -n1"
        " | cut -c 7-");

    return std::stod(watts);
  }
};

}  // namespace heat
