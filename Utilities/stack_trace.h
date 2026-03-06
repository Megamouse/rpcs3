#pragma once
#include <util/types.hpp>
#include <util/logs.hpp>

#ifdef __cpp_lib_stacktrace
#include <stacktrace>
#endif

namespace utils
{
	namespace stack_trace
	{
		// Printing utilities

		template <typename T>
		concept Logger = requires (T& t, const std::string& msg)
		{
			{ t.print(msg) };
		};

		struct print_to_log
		{
			logs::channel& log;

		public:
			print_to_log(logs::channel& chan)
				: log(chan)
			{}

			void print(const std::string& s)
			{
				log.error("%s", s);
			}
		};
	}

	std::vector<void*> get_backtrace(int max_depth = 255);
	std::vector<std::string> get_backtrace_symbols(const std::vector<void*>& stack);

	FORCE_INLINE std::string get_stacktrace()
	{
#ifdef __cpp_lib_stacktrace
		std::ostringstream oss;
		oss << std::stacktrace::current();
		oss << std::endl;
		oss << "vs" << std::endl;
		oss << std::endl;
		const auto trace = get_backtrace(255);
		const auto lines = get_backtrace_symbols(trace);
		oss << fmt::merge(lines, "\n");
		return oss.str();
#endif
	}

	FORCE_INLINE void print_trace(stack_trace::Logger auto& logger, [[maybe_unused]] int max_depth = 255)
	{
#ifdef __cpp_lib_stacktrace
		const std::string bt = get_stacktrace();
		const auto lines = fmt::split(bt, "\n");
#else
		const auto trace = get_backtrace(max_depth);
		const auto lines = get_backtrace_symbols(trace);
#endif

		for (const auto& line : lines)
		{
			logger.print(line);
		}
	}
}
