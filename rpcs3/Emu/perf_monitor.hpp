#pragma once

#include "util/types.hpp"

LOG_CHANNEL(abc_log, "ABC");

struct perf_monitor
{
public:
	void operator()();
	~perf_monitor();

	static constexpr auto thread_name = "Performance Sensor"sv;

	void get_data(double& total_usage, std::vector<double>& per_core_usage);

private:
	void set_data(double total_usage, std::vector<double> per_core_usage);

	std::mutex m_mutex;
	atomic_t<bool> m_dirty{true};
	double m_total_usage = 0.0;
	std::vector<double> m_per_core_usage;
};
