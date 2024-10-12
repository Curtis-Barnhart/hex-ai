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

    int ecode = Util::combine_games(inputs, output, error);
    std::printf("error code: %d\n", ecode);
    return 0;
}

