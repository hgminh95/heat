#include "heat/method.h"

#include <atomic>

namespace heat {

struct ICC final : public HeatMethod<ICC> {
  std::string name() const noexcept override {
    return "icc";
  }

  void RunImpl() noexcept override {
    x += y;
    if (x > 2)
      y += z;
    if (z < 3)
      z -= x;
  }

  std::atomic<int> x{1};
  std::atomic<int> y{1};
  std::atomic<int> z{1};
};

}  // namespace heat
