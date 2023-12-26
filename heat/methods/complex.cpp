#include "heat/method.h"

#include <atomic>

namespace heat {

struct Complex final : public HeatMethod<Complex> {
  Complex() {
    av.resize(16, 1);
    bv.resize(16, 2);
    cv.resize(16, 3);
  }

  std::string name() const noexcept override {
    return "complex";
  }

  void RunImpl() noexcept override {
    for (int i = 0; i < 16; ++i) {
      av[i] = av[i] / bv[i] + cv[i];
      bv[i] = av[i] - bv[i] * cv[i];
      cv[i] = av[i] * bv[i] - cv[i];

      ia += ia / 2 + ia / ib - id + ie;
      ib -= ib * 2;
      ic /= id * 7 + ic;
      id *= id * 2 + id % ie;
      ie += 3;

      fa += fa / 2.1 + fa / fb - fd + fe;
      fb -= fb * 1.5;
      fc /= fd * 0.7 + fc;
      fd *= fd * 1.3;
    }
  }

  std::vector<uint32_t> av, bv, cv;

  double fa;
  double fb;
  double fc;
  double fd;
  double fe;

  int ia;
  int ib;
  int ic;
  int id;
  int ie;
};

}  // namespace heat
