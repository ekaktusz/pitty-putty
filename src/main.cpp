#include "GameApplication.h"

#include <filesystem>
#include <memory>

#include <bagla-engine/asset-manager/AssetManager.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

void initializeLogger(spdlog::level::level_enum logLevel, std::string_view logFilePath)
{
	std::vector<spdlog::sink_ptr> sinks;

	// Create file sink (always used)
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.data());
	file_sink->set_level(logLevel);
	file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
	sinks.push_back(file_sink);

#ifndef WSL
	// in WSL there is an issue with audio errors, which spams the console
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(logLevel);
	console_sink->set_pattern("[%^%l%$] %v");
	sinks.push_back(console_sink);
#endif

	auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());
	logger->set_level(logLevel);
	logger->flush_on(logLevel);
	spdlog::set_default_logger(logger);

	spdlog::set_pattern("%H:%M:%S:%e [%^ %l %$] %s:%# %!(): %v");

#ifdef WSL
	SPDLOG_INFO("Logger initialized (WSL mode - logging to file only)");
#else
	SPDLOG_INFO("Logger initialized (logging to console and file)");
#endif
	logger->flush();
}

int main(int argc, char* argv[])
{
	initializeLogger(spdlog::level::debug, "pitty-putty.log");

	GameApplication gameApplication;
	gameApplication.run();
	return 0;
}
