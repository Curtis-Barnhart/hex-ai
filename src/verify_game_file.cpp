/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdio>
#include <string>

#include "hex-ai/Util/FileIO/file_types.hpp"

int main (int argc, char *argv[]) {
    for (int x = 1; x < argc; x++) {
        Util::FileIO::info_file(std::string(argv[x]));
    }
    return 0;
}

