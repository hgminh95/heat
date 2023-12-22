#include "heat/method.h"

namespace heat {

struct Simple final : public HeatMethod<Simple> {
  std::string name() const noexcept override {
    return "simple";
  }

  __attribute__((noinline)) void RunImpl() noexcept override {
    x = 100;
  }

  volatile int x{0};
};

}  // namespace heat
