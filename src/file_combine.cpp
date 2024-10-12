#include "hex-ai/Util/generated_games_combiner.hpp"

#include <cstdio>
#include <string>
#include <vector>


int main (int argc, char *argv[]) {
    std::vector<std::string> inputs;
    for (int x = 1; x < argc - 1; x++) {
        inputs.emplace_back(argv[x]);
    }
    std::string output(argv[argc - 1]), error;

    unsigned int ecode = Util::combine_games(inputs, output, error);
    std::printf("hex-ai: file_combine: error code: %d\n", ecode);
    if (ecode) {
        std::printf("hex-ai: file_combine: %s", error.c_str());
    }

    return 0;
}

