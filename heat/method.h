#pragma once

#include "lemire_pc/event_counter.h"

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>

#include <glog/logging.h>

namespace heat {

struct IHeatMethod {
 public:
  virtual ~IHeatMethod() = default;

  void RunForever() noexcept {
    while (1) {
      RunImpl();
    }
  }

  void Run(std::atomic<bool>& active) noexcept {
    event_collector collector;

    event_aggregate aggregate{};
    bool in_active_period{false};
    while (!stop_) {
      if (active) {
        if (!in_active_period) {
          in_active_period = true;
          collector.start();
        }
        for (int i = 0; i < 100; ++i) {
          RunImpl();
        }
      } else {
        if (in_active_period) {
          in_active_period = false;
          event_count allocate_count = collector.end();
          aggregate << allocate_count;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }
    LOG(INFO) << "Elapsed: " << aggregate.elapsed_ns();
    LOG(INFO) << "Instruction: " << aggregate.instructions();
    LOG(INFO) << "Cycle: " << aggregate.cycles();
    LOG(INFO) << "IPC: " << aggregate.instructions() / aggregate.cycles();
  }

  virtual void RunImpl() noexcept = 0;

  void Stop() {
    stop_ = true;
  }

 private:
  std::atomic<bool> stop_{false};
};

class HeatMethodManager {
 public:
  template <typename T>
  static void Register() {
    T t;
    classes_[t.name()] = []() {
      return static_cast<IHeatMethod*>(new T());
    };
  }

  static std::unique_ptr<IHeatMethod> Make(std::string name);

 private:
  static std::unordered_map<std::string, std::function<IHeatMethod*()>> classes_;
};

template <typename T, T>
struct ForceInit {};

template <typename T>
struct HeatMethod : public IHeatMethod {
 public:
  virtual std::string name() const noexcept = 0;

 private:
  static int reg_;
  using ForceInitT = ForceInit<int&, reg_>;

  static int Init() {
    HeatMethodManager::Register<T>();
    return 0;
  }
};

template <typename T>
int HeatMethod<T>::reg_ = HeatMethod<T>::Init();

}  // namespace heat
