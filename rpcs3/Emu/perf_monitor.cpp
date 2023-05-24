#include "stdafx.h"
#include "perf_monitor.hpp"

#include "Emu/System.h"
#include "util/cpu_stats.hpp"
#include "Utilities/Thread.h"

LOG_CHANNEL(perf_log, "PERF");

void perf_monitor::operator()()
{
	constexpr u64 update_interval_us = 1000000; // Update every second
	constexpr u64 log_interval_us = 10000000;   // Log every 10 seconds
	u64 elapsed_us = 0;

	double total_usage = 0.0;
	std::vector<double> per_core_usage;

	utils::cpu_stats stats;
	stats.init_cpu_query();
	stats.get_per_core_usage(per_core_usage, total_usage);
	set_data(total_usage, per_core_usage);

	u32 logged_pause = 0;
	u64 last_pause_time = umax;

	std::vector<double> per_core_usage;

	while (thread_ctrl::state() != thread_state::aborting)
	{
		thread_ctrl::wait_for(update_interval_us);
		elapsed_us += update_interval_us;

		if (thread_ctrl::state() == thread_state::aborting)
		{
			break;
		}

		double total_usage = 0.0;

		stats.get_per_core_usage(per_core_usage, total_usage);
		set_data(total_usage, per_core_usage);

		if (elapsed_us >= log_interval_us)
		{
			elapsed_us = 0;

			const bool is_paused = Emu.IsPaused();
			const u64 pause_time = Emu.GetPauseTime();

			if (!is_paused || last_pause_time != pause_time)
			{
				// Resumed or not paused since last check
				logged_pause = 0;
				last_pause_time = pause_time;
			}

			if (is_paused)
			{
				if (logged_pause >= 2)
				{
					// Let's not spam the log when emulation is paused
					// But still emit the message two times so even paused state can be debugged and inspected
					continue;
				}

				logged_pause++;
			}

			std::string msg = fmt::format("CPU Usage: Total: %.1f%%", total_usage);

			if (!per_core_usage.empty())
			{
				fmt::append(msg, ", Cores:");
			}

			for (size_t i = 0; i < per_core_usage.size(); i++)
			{
				fmt::append(msg, "%s %.1f%%", i > 0 ? "," : "", per_core_usage[i]);
			}

			perf_log.notice("%s", msg);
		}
	}
}

perf_monitor::~perf_monitor()
{
}

void perf_monitor::get_data(double& total_usage, std::vector<double>& per_core_usage)
{
	if (m_dirty.exchange(false)) [[unlikely]]
	{
		std::lock_guard lock(m_mutex);
		total_usage = m_total_usage;
		per_core_usage = m_per_core_usage;
	}
}

void perf_monitor::set_data(double total_usage, std::vector<double> per_core_usage)
{
	std::lock_guard lock(m_mutex);
	m_total_usage = total_usage;
	m_per_core_usage = std::move(per_core_usage);
	m_dirty = true;
}
