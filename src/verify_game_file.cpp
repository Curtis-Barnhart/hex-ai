#include <cstdio>
#include <hex-ai/GameState/HexState.hpp>
#include <string>

#include "hex-ai/Util/FileIO/file_types.hpp"

int main (int argc, char *argv[]) {
    for (int x = 1; x < argc; x++) {
        Util::FileIO::info_file(std::string(argv[x]));
    }
    return 0;
}

