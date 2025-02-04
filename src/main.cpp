#include "GameApplication.h"
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <string> // Added to fix VCIC001

void initializeLogger()
{
	std::vector<spdlog::sink_ptr> sinks;

	// Create file sink (always used)
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("pitty_putty.log");
	file_sink->set_level(spdlog::level::debug);
	file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
	sinks.push_back(file_sink);

#ifndef WSL
	// Create console sink if not running on WSL
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::debug);
	console_sink->set_pattern("[%^%l%$] %v");
	sinks.push_back(console_sink);
#endif

	// Create logger with the appropriate sinks
	auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());
	logger->set_level(spdlog::level::debug);
	logger->flush_on(spdlog::level::debug);
	// Set as default logger
	spdlog::set_default_logger(logger);

#ifdef WSL
	SPDLOG_INFO("Logger initialized (WSL mode - logging to file only)");
#else
	SPDLOG_INFO("Logger initialized (logging to console and file)");
#endif
	logger->flush();
}

int main(int argc, char* argv[])
{
	initializeLogger();
	//spdlog::set_pattern("*** %H:%M:%S [%^ %l %$] %s:%# %!(): %v");
	SPDLOG_INFO("asd {}", std::filesystem::absolute(argv[0]).string());
	//SPDLOG_INFO(std::filesystem::absolute(argv[0]));
	GameApplication gameApplication;
	gameApplication.run();
	return 0;
}
