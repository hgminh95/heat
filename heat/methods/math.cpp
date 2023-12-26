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

struct ArithmeticInteger final : public HeatMethod<ArithmeticInteger> {
  std::string name() const noexcept override {
    return "arithmetic_integer";
  }

  void RunImpl() noexcept override {
    a += a / 2 + a / b - d + e;
    b -= b * 2;
    c /= d * 7 + c;
    d *= d * 2 + d % e;
    e += 3;
  }

 private:
  thread_local static int a;
  thread_local static int b;
  thread_local static int c;
  thread_local static int d;
  thread_local static int e;
};

thread_local int ArithmeticInteger::a{1};
thread_local int ArithmeticInteger::b{1};
thread_local int ArithmeticInteger::c{1};
thread_local int ArithmeticInteger::d{1};
thread_local int ArithmeticInteger::e{1};

struct ArithmeticFloat final : public HeatMethod<ArithmeticFloat> {
  std::string name() const noexcept override {
    return "arithmetic_float";
  }

  void RunImpl() noexcept override {
    a += a / 2.1 + a / b - d + e;
    b -= b * 1.5;
    c /= d * 0.7 + c;
    d *= d * 1.3 + d / e;
    e += 3;
  }

 private:
  thread_local static double a;
  thread_local static double b;
  thread_local static double c;
  thread_local static double d;
  thread_local static double e;
};

thread_local double ArithmeticFloat::a{1.5};
thread_local double ArithmeticFloat::b{1.5};
thread_local double ArithmeticFloat::c{1.5};
thread_local double ArithmeticFloat::d{1.5};
thread_local double ArithmeticFloat::e{1.5};

struct ArithmeticFloatArray final : public HeatMethod<ArithmeticFloatArray> {
  ArithmeticFloatArray() {
    a.resize(100'000, 1.5);
    b.resize(100'000, 1.5);
    c.resize(100'000, 1.5);
    d.resize(100'000, 1.5);
    e.resize(100'000, 1.5);
  }

  std::string name() const noexcept override {
    return "arithmetic_float_array";
  }

  void RunImpl() noexcept override {
    i += i * 2;
    i %= a.size();
    fa += fa / 2.1 + fa / fb - fd + fe;
    a[i] += a[i] / 2.1 + a[i] / b[i] - d[i] + e[i];
    fb -= fb * 1.5;
    b[i] -= b[i] * 1.5;
    fc /= fd * 0.7 + fc;
    c[i] /= d[i] * 0.7 + c[i];
    fd *= fd * 1.3;
    d[i] *= d[i] * 1.3 + d[i] / e[i];
    e[i] += 3;
  }

 private:
  int i = 0;
  std::vector<double> a;
  std::vector<double> b;
  std::vector<double> c;
  std::vector<double> d;
  std::vector<double> e;

  double fa{1.5};
  double fb{1.5};
  double fc{1.5};
  double fd{1.5};
  double fe{1.5};
};

}  // namespace heat
