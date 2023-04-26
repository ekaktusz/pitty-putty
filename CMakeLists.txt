# My template for simple C++ projects, using vcpkg as main package manager

cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)

# git submodule add https://github.com/microsoft/vcpkg vendor/vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(PittyPutty)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
)

add_executable(PittyPutty ${SOURCES})
add_subdirectory(vendor/bagla-engine)

target_include_directories(PittyPutty PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/bagla-engine/include" )

find_package(SFML COMPONENTS system window graphics audio network CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Tweeny CONFIG REQUIRED)

target_link_libraries(PittyPutty PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
target_link_libraries(PittyPutty PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(PittyPutty PRIVATE bagla-engine)
target_link_libraries(PittyPutty PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(PittyPutty PRIVATE tweeny)

# target_precompile_headers(ProjectName PUBLIC "src/pch.h")
