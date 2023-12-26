#include "heat/method.h"
#include "heat/cpuinfo.h"

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <thread>
#include <vector>
#include <chrono>
#include <numeric>

DEFINE_bool(bm, false, "Run in benchmark mode, display the temparature and power consumption");
DEFINE_string(method, "simple", "Method used to heat up your laptop");
DEFINE_int32(cores, 1, "Number of cores used");
DEFINE_int32(runs, 2, "Number of runs - only in bm mode");

using namespace std::literals::chrono_literals;

void RunForever(heat::IHeatMethod *method) {
  heat::CpuInfo cpu_info;
  std::vector<std::thread> threads;

  for (int i = 0; i < FLAGS_cores; ++i) {
    threads.emplace_back([method]() {
      method->RunForever();
    });
  }

  auto initial_temp = cpu_info.GetCpuTemp();
  auto start_time = std::chrono::system_clock::now();

  while (1) {
    auto curr_temp = cpu_info.GetCpuTemp();
    auto now = std::chrono::system_clock::now();
    LOG(INFO) << "CPU Temparature (Celcius): " << curr_temp
        << " (+" << (curr_temp - initial_temp) << ")"
        << " - " << std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count() << "s";
    LOG(INFO) << "Adapter Power (Watt): " << cpu_info.GetAdapterPowerWatt();
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
}

struct Stats {
  Stats(std::vector<float> values) {
    if (values.size() == 0)
      return;

    sample = values.size();

    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    avg = sum / sample;

    double sq_sum = std::inner_product(values.begin(), values.end(), values.begin(), 0.0);
    std = std::sqrt(sq_sum / sample - avg * avg);

    min = *std::min_element(values.begin(), values.end());
    max = *std::max_element(values.begin(), values.end());

    std::sort(values.begin(), values.end());
    p10 = values[sample * 10 / 100];
    p50 = values[sample * 50 / 100];
    p90 = values[sample * 90 / 100];
  }

  std::string ToStr() {
    return "sample=" + std::to_string(sample)
        + " avg=" + std::to_string(avg)
        + " std=" + std::to_string(std)
        + " min=" + std::to_string(min)
        + " max=" + std::to_string(max)
        + " p10=" + std::to_string(p10)
        + " p50=" + std::to_string(p50)
        + " p90=" + std::to_string(p90);
  }

  int sample{0};
  float avg{0};
  float min{0};
  float max{0};
  float p10{0};
  float p50{0};
  float p90{0};
  float std{0};
};

std::pair<Stats, Stats> GetBaseline() {
  heat::CpuInfo cpu_info;
  auto start_time = std::chrono::system_clock::now();

  std::vector<float> temps;
  std::vector<float> watts;

  // Run in 1 minutes
  while (1) {
    temps.push_back(cpu_info.GetCpuTemp());
    watts.push_back(cpu_info.GetAdapterPowerWatt());
    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (std::chrono::system_clock::now() - start_time > 1min)
      break;
  }

  return {Stats(temps), Stats(watts)};
}

void RunBenchmark(heat::IHeatMethod *method) {
  LOG(INFO) << "Getting baseline temperature and power usage. Please check to make sure the numbers are stable.";
  LOG(INFO) << "Will be finished in 1min";
  auto [temps, watts] = GetBaseline();

  LOG(INFO) << "Baseline temp: " << temps.ToStr();
  LOG(INFO) << "Baseline watt: " << watts.ToStr();

  std::atomic<bool> active;
  std::vector<std::thread> threads;

  for (int i = 0; i < FLAGS_cores; ++i) {
    threads.emplace_back([method, &active]() {
      method->Run(active);
    });
  }

  heat::CpuInfo cpu_info;
  for (int i = 0; i < FLAGS_runs; ++i) {
    LOG(INFO) << "Run #" << i;
    // Wait for 30s to cool down
    std::this_thread::sleep_for(std::chrono::seconds(30));

    auto initial_temp = cpu_info.GetCpuTemp();
    float first_5s_delta = 0;
    float first_10s_delta = 0;
    float first_15s_delta = 0;
    active = true;

    std::vector<float> temps;
    std::vector<float> watts;

    // Get 30 samples
    for (int j = 0; j < 30; ++j) {
      std::this_thread::sleep_for(std::chrono::seconds(5));

      auto temp = cpu_info.GetCpuTemp();
      auto watt = cpu_info.GetAdapterPowerWatt();

      // Power measurement is lagged behind, so we skip some samples at the beginning
      if (j > 20) {
        watts.push_back(watt);
      }
      temps.push_back(temp);

      if (j == 0) {
        first_5s_delta = temp - initial_temp;
      } else if (j == 1) {
        first_10s_delta = temp - initial_temp;
      } else if (j == 2) {
        first_15s_delta = temp - initial_temp;
      }
    }

    active = false;

    LOG(INFO) << "Initial Temp: " << initial_temp;
    LOG(INFO) << "  5s:  +" << first_5s_delta;
    LOG(INFO) << "  10s: +" << first_10s_delta;
    LOG(INFO) << "  15s: +" << first_15s_delta;
    LOG(INFO) << "Temps: " << Stats(temps).ToStr();
    LOG(INFO) << "Watts: " << Stats(watts).ToStr();
  }

  method->Stop();

  for (auto &thread : threads) {
    thread.join();
  }
}

int main(int argc, char *argv[]) {
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  LOG(INFO) << "Emit heat using method " << FLAGS_method << " in " << FLAGS_cores << " core(s)";
  auto method = heat::HeatMethodManager::Make(FLAGS_method);
  if (FLAGS_bm) {
    LOG(INFO) << "Running in benchmark mode";
    RunBenchmark(method.get());
  } else {
    LOG(INFO) << "Press Ctrl+C to exit";
    RunForever(method.get());
  }

  return 0;
}

