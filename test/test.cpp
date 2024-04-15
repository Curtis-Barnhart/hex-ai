/**
* This is just to test that the LRUCache is actually working,
* because there war a lot of pointer arithmetic involved and
* I'd gotten some of it wrong before.
*/

#include <cstdlib>
#include <iostream>
#include <string>

#include "../src/GameSolve/LRUCache.hpp"

#define STR_LEN 3
#define STR_GEN 2000000
#define CACHE_S 500

char alph[27] = "abcdefghijklmnopqrstuvwxyz";

int hits = 0;
std::string nowhere;

int main(int argc, char *argv[]) {
    char s_build[STR_LEN];
    std::string s;
    Cache::LRUCache<std::string, std::string> test(CACHE_S);

    for (int x = 0; x < STR_GEN; x++) {
        for (int len = 0; len < STR_LEN; len++) {
            s_build[len] = alph[std::rand() % 26];
        }
        s.assign(s_build, STR_LEN);
        if (!test.lookup(s, s)) {
            test.insert(s, s);
        } else {
            hits++;
        }
    }

    std::cout << hits << " cache hits\n";

    return 0;
}

