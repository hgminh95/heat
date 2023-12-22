#include "heat/method.h"

#include <glog/logging.h>

namespace heat {

std::unordered_map<std::string, std::function<IHeatMethod*()>> HeatMethodManager::classes_;

std::unique_ptr<IHeatMethod> HeatMethodManager::Make(std::string name) {
  auto it = classes_.find(name);
  CHECK(it != classes_.end()) << "Unknown heat method '" << name << "'";
  return std::unique_ptr<IHeatMethod>(it->second());
}

}  // namespace heat
