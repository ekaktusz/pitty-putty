#!/bin/bash

# Set environment variables
export VCPKG_MAX_CONCURRENCY=8
export VCPKG_BINARY_SOURCES="clear;files,$HOME/.vcpkg-cache,readwrite"

WSL_ON_OFF_FLAG="OFF"

if [[ $(grep -i Microsoft /proc/version) ]]; then
    WSL_ON_OFF_FLAG="ON"
    echo "Bash is running on WSL"
fi

if [ $# -eq 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
          -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
          -DWSL=${WSL_ON_OFF_FLAG} \
          -H. -Bout/build -G Ninja
    ninja -C out/build
    mkdir -p .vscode
    cp out/build/compile_commands.json ./compile_commands.json
fi

if [[ $1 == "clean" ]]; then
    cmake --build out/build --target clean
    rm -rf out/*
    echo "Project cleaned."
fi