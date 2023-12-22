#include "heat/method.h"

namespace heat {

struct Simd final : public HeatMethod<Simd> {
  Simd() {
    a.resize(16, 1);
    b.resize(16, 2);
    c.resize(16, 3);
  }

  std::string name() const noexcept override {
    return "simd";
  }

  void RunImpl() noexcept override {
    for (int i = 0; i < 16; ++i) {
      a[i] = a[i] / b[i] + c[i];
      b[i] = a[i] - b[i] * c[i];
      c[i] = a[i] * b[i] - c[i];
    }
  }

  std::vector<double> a, b, c;
};

}  // namespace heat
