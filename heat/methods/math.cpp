#include "heat/method.h"

namespace heat {

struct Math final : public HeatMethod<Math> {
  std::string name() const noexcept override {
    return "math";
  }

  void RunImpl() noexcept override {
    x += x / 15 + 3;
    y -= y * y + 2;
    z *= z + 3;
    l %= (l + z + 4);
    l += 50;
    m += m / 2;
    n -= n * 1.5 + 10;
  }

 private:
  int x{0};
  int y{0};
  int z{1};
  int l{9};
  int m{10};
  int n{10};
};

}  // namespace heat
