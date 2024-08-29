#include "Game.h"
#include <iostream>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

void setup_logger()
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
	spdlog::info("Logger initialized (WSL mode - logging to file only)");
#else
	spdlog::info("Logger initialized (logging to console and file)");
#endif
	logger->flush();
}

int main()
{

	setup_logger();
	Game game;
	game.run();
	return 0;
}
