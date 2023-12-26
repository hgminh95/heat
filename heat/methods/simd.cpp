#include "heat/method.h"

namespace heat {

template <size_t Size>
struct Simd final : public HeatMethod<Simd<Size>> {
  Simd() {
    a.resize(Size, 1);
    b.resize(Size, 2);
    c.resize(Size, 3);
  }

  std::string name() const noexcept override {
    return "simd" + std::to_string(Size);
  }

  void RunImpl() noexcept override {
    for (int i = 0; i < Size; ++i) {
      a[i] = a[i] / b[i] + c[i];
      b[i] = a[i] - b[i] * c[i];
      c[i] = a[i] * b[i] - c[i];
    }
  }

  std::vector<uint32_t> a, b, c;
};

template struct Simd<16>;
template struct Simd<32>;
template struct Simd<64>;

}  // namespace heat
