"""
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci
"""
# SPDX-License-Identifier: GPL-3.0-or-later

project_name = "encoder"


import sys

part1 = ""
part1 += "# Set minimum required version of CMake\n"
part1 += "cmake_minimum_required(VERSION 3.12)\n\n"

part1 += "#include build functions from Pico SDK\n"
part1 += "include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)\n"


part2 = ""
part2 += "set(CMAKE_C_STANDARD 11)\n"
part2 += "set(CMAKE_CXX_STANDARD 17)\n\n"

part2 += "# Creates a pico-sdk subdirectory in our project for the libraries\n"
part2 += "pico_sdk_init()\n\n"

part2 += "# point out the CMake, where to find the executable source file\n"
part2 += "add_executable(${PROJECT_NAME}\n"
part2 += "        main.c\n"
part2 += ")\n\n"

part2 += "# create map/bin/hex/uf2 files.\n"
part2 += "pico_add_extra_outputs(${PROJECT_NAME})\n"

part2 += "# Pull in our pico_stdlib which pulls in commonly used features (gpio, timer-delay etc)\n"
part2 += "target_link_libraries(${PROJECT_NAME}\n"
part2 += "            pico_stdlib\n"
part2 += ")\n"

cmake_str = part1 + f"\nproject({project_name} C CXX ASM)\n" + part2

# write to CMakeLists.txt
try:
	with open("CMakeLists.txt", "w+") as file:
		file.write(cmake_str)
except:
	print("Cannot write to CMakeLists.txt")
	exit()
print("Generated CMakeLists.txt.")

