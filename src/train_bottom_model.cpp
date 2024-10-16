/*
 * Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
 *
 * This file is part of hex-ai.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include <tiny_dnn/tiny_dnn.h>

#include "hex-ai/GameState/HexState.hpp"
#include "hex-ai/GameState/io.hpp"

void hex_to_vec(const GameState::HexState &state, tiny_dnn::vec_t &vec) {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            vec.push_back(state.at(x, y));
        }
    }
}

void hexs_to_vecs(const std::vector<GameState::HexState> &states, std::vector<tiny_dnn::vec_t> &vecs) {
    for (const GameState::HexState &state : states) {
        vecs.emplace_back();
        hex_to_vec(state, *(vecs.end() - 1));
    }
}

int main (int argc, char *argv[]) {
    tiny_dnn::network<tiny_dnn::sequential> net;
    net << tiny_dnn::fully_connected_layer(121, 20)  << tiny_dnn::activation::relu()
        << tiny_dnn::fully_connected_layer(20, 20) << tiny_dnn::activation::relu()
        << tiny_dnn::fully_connected_layer(20, 2)  << tiny_dnn::activation::softmax();
    
    std::vector<tiny_dnn::vec_t> inputs;
    std::vector<tiny_dnn::label_t> labels;

    std::vector<GameState::HexState> hex_states;
    std::vector<bool> bools;

    std::string hex_filename("/home/curtisb/temporary_hex/batch01_hex00001");
    std::string bool_filename("/home/curtisb/temporary_hex/batch01_bool00001");

    int status = GameState::read_hexstates(hex_filename, hex_states);
    if (status) {
        std::cout << "error: " << status << "\n";
    }
    std::cout << "HexState read in: " << hex_states.size() << "\n";
    hexs_to_vecs(hex_states, inputs);

    std::ifstream bool_file(bool_filename);
    char c;
    std::cout << "bool stream status: " << bool_file.rdstate() << "\n";
    for (std::size_t x = 0; x < hex_states.size(); x++) {
        bool_file.get(c);
        labels.push_back(c == '1' ? 1 : 0);
    }
    bool_file.close();

    std::cout << hex_states.size() << ',' << bools.size() << '\n';

    size_t batch_size = 1;
    int epochs = 200;
    tiny_dnn::adagrad opt;
    for (int x = 0; x < epochs; x++) {
        net.train<tiny_dnn::mse>(opt, inputs, labels, batch_size, 100);
        auto test_result = net.test(inputs, labels);
        test_result.print_detail(std::cout);
    }

    return 0;
}

