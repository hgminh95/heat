#include "heat/method.h"

namespace heat {

struct Empty final : public HeatMethod<Empty> {
  std::string name() const noexcept override {
    return "empty";
  }

  void RunImpl() noexcept override {
  }
};

}  // namespace heat
