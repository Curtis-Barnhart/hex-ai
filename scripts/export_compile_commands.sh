#!/bin/bash

cpp_ai_comp_settings="-DCPP_AI_TESTS=ON"

echo "CPP-AI: Making temporary build directory..."
cd build
mkdir compile_commands_temp
cd compile_commands_temp

echo "CPP-AI: Running CMake with settings:"
echo "cmake ../.. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $cpp_ai_comp_settings"
cmake ../.. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 $cpp_ai_comp_settings

echo "CPP-AI: Copying compile_commands.json..."
cp compile_commands.json ../..

echo "CPP-AI: Cleaning up directory..."
cd ..
rm -r compile_commands_temp

