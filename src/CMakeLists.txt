add_executable(
    main
    main.cpp
    GameState/HexState.cpp
    GameSolve/AlphaBeta.cpp
    Util/LRUCache.hpp
)

# we want to add a compilation feature to one of our targets (main).
# we add a feature that uses C++20.
target_compile_features(
    main
    PRIVATE
    cxx_std_20
)

# we want to set some compilation options (for how many warnings to show).
target_compile_options(
    main 
    PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

