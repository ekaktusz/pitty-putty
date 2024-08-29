# My template for simple C++ projects, using vcpkg as main package manager

cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# git submodule add https://github.com/microsoft/vcpkg vendor/vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(PittyPutty)

 # To copy .dll files in the output folder. (tmxlite)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB SOURCES
    src/*.h
    src/*.cpp
)

add_executable(PittyPutty ${SOURCES})

# Setup b�gla
add_subdirectory(vendor/bagla-engine)
target_include_directories(PittyPutty PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/bagla-engine/include" )

find_package(SFML COMPONENTS system window graphics audio network CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(effolkronium_random CONFIG REQUIRED)

if (WIN32)
    # For Visual Studio hot reload
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/ZI>)
    #set(CMAKE_CXX_FLAGS_DEBUG "/MDd /ZI /Ob0 /Od /RTC1")
endif()

target_link_libraries(PittyPutty PRIVATE
    sfml-system sfml-network sfml-graphics sfml-window sfml-audio
    spdlog::spdlog spdlog::spdlog_header_only
    bagla-engine
    effolkronium_random
)

message(" wsl:  ${WSL}")

#target_precompile_headers(ProjectName PUBLIC "src/pch.h")
