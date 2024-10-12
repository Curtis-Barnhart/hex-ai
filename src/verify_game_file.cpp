#include "hex-ai/GameState/io.hpp"

#include <cstdio>
#include <vector>

int main (int argc, char *argv[]) {
    std::vector<GameState::HexState> hex;

    int error = GameState::read_hexstates(std::string(argv[1]), hex);

    std::printf("Error Code: %d\n", error);
    std::printf("Games Present: %zu\n", hex.size());

    return 0;
}

