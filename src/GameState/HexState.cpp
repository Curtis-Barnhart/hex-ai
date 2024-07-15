#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

#include "hex-ai/GameState/HexState.hpp"

using GameState::HexState;
using HexState::PLAYERS::PLAYER_NONE;
using HexState::PLAYERS::PLAYER_ONE;
using HexState::PLAYERS::PLAYER_TWO;

/*************************************************
 * Methods for HexState::HexActReader            *
 ************************************************/

HexState::HexActReader::HexActReader(const HexState &state) {
    if (state.who_won() != PLAYER_NONE) {
        this->valid = false;
        this->a.x = BOARD_SIZE;
        return;
    }
    this->book = &state;
    this->a.whose = state.turn == 0 ? PLAYER_ONE : PLAYER_TWO;
    if (state.board[0][0] == PLAYER_NONE) {
        this->valid = true;
    } else {
        this->valid = true;
        (*this)++;
    }
}

const HexState::Action &HexState::HexActReader::get_action() const {
    return this->a;
}

HexState::HexActReader::operator bool() const {
    return this->valid;
}

HexState::HexActReader &HexState::HexActReader::operator++(int) {
    do {
        if (this->a.x >= BOARD_SIZE) {
            this->valid = false;
            return *this;
        }
        this->a.y++;
        if (this->a.y == BOARD_SIZE) {
            if (++(this->a.x) >= BOARD_SIZE) {
                this->valid = false;
                return *this;
            }
            this->a.y = 0;
        }
    } while (book->board[this->a.x][this->a.y] != HexState::PLAYERS::PLAYER_NONE);
    return *this;
}

/*************************************************
 * Methods for HexState                          *
 ************************************************/

HexState::HexState() {
    this->board = new unsigned char[BOARD_SIZE][BOARD_SIZE];
    std::fill<unsigned char *, unsigned char>(&this->board[0][0], &this->board[0][0] + BOARD_SIZE * BOARD_SIZE, PLAYER_NONE);
}

HexState::HexState(HexState &&other) {
    this->turn = other.turn;
    this->board = other.board;
    other.board = nullptr;
}

HexState::HexState(const HexState &other) {
    this->turn = other.turn;
    if (this->board == nullptr) {
        this->board = new unsigned char[BOARD_SIZE][BOARD_SIZE];
    }
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            this->board[x][y] = other.board[x][y];
        }
    }
    // I'm pretty sure this works but I should check somewhere
    // std::memcpy(&this->board[0][0], &other.board[0][0], sizeof(unsigned char) * BOARD_SIZE * BOARD_SIZE);
}

HexState &HexState::operator=(const HexState &other) {
    this->turn = other.turn;
    // I'm pretty sure this works but I should check somewhere
    // std::memcpy(&this->board[0][0], &other.board[0][0], sizeof(unsigned char) * BOARD_SIZE * BOARD_SIZE);
    if (this->board == nullptr) {
        this->board = new unsigned char[BOARD_SIZE][BOARD_SIZE];
    }
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            this->board[x][y] = other.board[x][y];
        }
    }
    return *this;
}

HexState &HexState::operator=(HexState &&other) {
    if (this->board != nullptr) {
        delete[] this->board;
    }
    this->turn = other.turn;
    this->board = other.board;
    other.board = nullptr;
    return *this;
}

HexState::~HexState() {
    if (this->board != nullptr) {
        delete[] this->board;
    }
}

bool HexState::operator==(const HexState &other) const {
    if (!this->board_state_equal(other)) {
        return false;
    }
    return this->turn == other.turn;
}

bool HexState::operator!=(const HexState &other) const {
    return !(*this == other);
}

bool HexState::board_state_equal(const HexState &other) const {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (this->board[x][y] != other.board[x][y]) {
                return false;
            }
        }
    }
    return true;
}

int HexState::whose_turn() const {
    return this->turn;
}

HexState::PLAYERS HexState::who_won() const {
    /* each char in each array denotes that the respective player owns the
     * tile at the x or y value on that border. The four chars represent
     * how many such tiles are actually owned in each array.*/
    unsigned char one_bottom[BOARD_SIZE], one_top[BOARD_SIZE], two_left[BOARD_SIZE], two_right[BOARD_SIZE],
                  one_bottom_i = 0      , one_top_i = 0      , two_left_i = 0      , two_right_i = 0      ;
    const int MAX_SIZE = BOARD_SIZE - 1;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (this->board[i][0] == PLAYER_ONE) {
            one_bottom[one_bottom_i++] = i;
        }
        if (this->board[i][MAX_SIZE] == PLAYER_ONE) {
            one_top[one_top_i++] = i;
        }
        if (this->board[0][i] == PLAYER_TWO) {
            two_left[two_left_i++] = i;
        }
        if (this->board[MAX_SIZE][i] == PLAYER_TWO) {
            two_right[two_right_i++] = i;
        }
    }

    // if any of a player's pieces on opposite edges connects, they win
    for (int bottom = 0; bottom < one_bottom_i; bottom++) {
        for (int top = 0; top < one_top_i; top++) {
            if (this->is_connected(one_bottom[bottom], 0, one_top[top], MAX_SIZE)) {
                return PLAYER_ONE;
            }
        }
    }
    for (int left = 0; left < two_left_i; left++) {
        for (int right = 0; right < two_right_i; right++) {
            if (this->is_connected(0, two_left[left], MAX_SIZE, two_right[right])) {
                return PLAYER_TWO;
            }
        }
    }
    return PLAYER_NONE;
}

void HexState::current_score_array(double score[2]) const {
    switch (this->who_won()) {
        case PLAYER_NONE:
            score[0] = 0.5l;
            score[1] = 0.5l;
            return;
        case PLAYER_ONE:
            score[0] = 1l;
            score[1] = 0l;
            return;
        case PLAYER_TWO:
            score[0] = 0l;
            score[1] = 1l;
            return;
    }
}

double HexState::current_score_double() const {
    switch (this->who_won()) {
        case PLAYER_ONE:
            return 1;
        case PLAYER_TWO:
            return 0;
        default:
            return 0.5;
    }
}

[[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
HexState *HexState::succeed(const Action &action) const {
    auto next = new HexState(*this);
    next->board[action.x][action.y] = action.whose;
    next->turn = (action.whose + 1) % 2;
    return next;
}

[[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
HexState *HexState::succeed(const Action &action, Action &baction) const {
    auto next = new HexState(*this);
    next->board[action.x][action.y] = action.whose;
    next->turn = (action.whose + 1) % 2;

    baction = action;
    return next;
}

HexState &HexState::succeed_in_place(const Action &action) {
    this->board[action.x][action.y] = action.whose;
    this->turn = (action.whose + 1) % 2;
    return *this;
}

HexState &HexState::succeed_in_place(const Action &action, Action &baction) {
    this->board[action.x][action.y] = action.whose;
    this->turn = (action.whose + 1) % 2;

    baction = action;
    return *this;
}

[[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
HexState *HexState::reverse(const Action &baction) const {
    auto previous = new HexState(*this);
    previous->board[baction.x][baction.y] = PLAYER_NONE;
    previous->turn = baction.whose;
    return previous;
}

HexState &HexState::reverse_in_place(const Action &baction) {
    this->board[baction.x][baction.y] = PLAYER_NONE;
    this->turn = baction.whose;
    return *this;
}

[[nodiscard("Discarding sole pointer to allocated memory would cause a leak.")]]
std::vector<HexState::Action> *HexState::get_actions() const {
    std::vector<Action> *a = new std::vector<Action>();
    this->get_actions(*a);    
    return a;
}

void HexState::get_actions(std::vector<Action> &buffer) const {
    // If someone has already won, there are no actions
    if (this->who_won() != PLAYER_NONE) {
        return;
    }

    unsigned char whose_turn = this->turn;
    buffer.reserve(BOARD_SIZE * BOARD_SIZE);
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (this->board[x][y] == PLAYER_NONE) {
                buffer.emplace_back(x, y, whose_turn);
            }
        }
    }
}

// TODO: there may be some io exceptions you wanna throw or do I dunno
[[nodiscard("Return value is an error code - do not discard.")]]
int HexState::pack_to_stream(std::ofstream &out) const {
    char pack4 = 0;
    int packed_in = 0;

    if (!out.good()) {
        return 1;
    }

    // pack in all the board states
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            pack4 |= (0x03 & this->board[x][y]);
            packed_in++;

            if (packed_in == 4) {
                packed_in = 0;
                out.put(pack4);
                pack4 = 0;
            } else {
                pack4 <<= 2;
            }
        }
    }

    // put in the current turn
    pack4 |= (0x03 & this->turn);
    packed_in++;
    pack4 <<= (2 * (4 - packed_in));
    out.put(pack4);

    return 0;
}

// TODO: there may be some io exceptions you wanna throw or do I dunno
[[nodiscard("Return value is an error code - do not discard.")]]
int HexState::unpack_from_stream(std::ifstream &in) {
    char pack4 = 0;
    int packed_in = 0;

    if (!in.good()) {
        return 3;
    }

    // get all board states
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (packed_in == 0) {
                in.get(pack4);
                if (in.eof()) {
                    return 1;
                }
                packed_in = 4;
            }

            this->board[x][y] = (pack4 & 0xc0) >> 6;
            if (
                this->board[x][y] != HexState::PLAYERS::PLAYER_ONE &&
                this->board[x][y] != HexState::PLAYERS::PLAYER_TWO &&
                this->board[x][y] != HexState::PLAYERS::PLAYER_NONE
            ) {
                return 2;
            }
            pack4 <<= 2;
            packed_in--;
        }
    }

    // get current turn
    if (packed_in == 0) {
        in.get(pack4);
        if (in.eof()) {
            return 1;
        }
        packed_in = 4;
    }

    this->turn = (pack4 & 0xc0) >> 6;
    if (
        this->turn != 0 &&
        this->turn != 1
    ) {
        return 2;
    }

    return 0;
}

// TODO: someday change these from being signed chars to ints
bool HexState::is_connected(signed char x1, signed char y1, signed char x2, signed char y2) const {
    const unsigned char COLOR = this->board[x1][y1];
    signed char neigh_x, neigh_y;
    std::vector<std::array<signed char, 2>> neighbors;
    bool visited[BOARD_SIZE][BOARD_SIZE] = {{ false }}, connected = false;
    visited[x1][y1] = true;
    std::vector<signed char> x_stack, y_stack;
    x_stack.push_back(x1);
    y_stack.push_back(y1);

    // for each tile we can touch (in the stack),
    // pop it off and add its neighbors to the stack if unvisited.
    while (x_stack.size() > 0) {
        signed char next_x = x_stack.back();
        signed char next_y = y_stack.back();
        x_stack.pop_back();
        y_stack.pop_back();

        // fill up neighbors buffer
        neighbors.clear();
        HexState::get_neighbors(next_x, next_y, neighbors);
        for (std::array<signed char, 2> &neighbor : neighbors) {
            neigh_x = neighbor[0], neigh_y = neighbor[1];
            if (this->board[neigh_x][neigh_y] == COLOR) {
                // if the touching piece is destination, perfect!
                if (neigh_x == x2 && neigh_y == y2) {
                    return true;
                }
                // otherwise, if we haven't seen it before, put on stack
                if (!visited[neigh_x][neigh_y]) {
                    visited[neigh_x][neigh_y] = true;
                    x_stack.push_back(neigh_x);
                    y_stack.push_back(neigh_y);
                }
            }
        }
    }

    return connected;
}

void HexState::get_neighbors(signed char x, signed char y, std::vector<std::array<signed char, 2>> &output) const {
    signed char neighbors[6][2] = {
        {static_cast<signed char>(x + 1),                          y     },
        {                         x     , static_cast<signed char>(y + 1)},
        {static_cast<signed char>(x - 1), static_cast<signed char>(y + 1)},
        {static_cast<signed char>(x - 1),                          y     },
        {                        x     , static_cast<signed char>(y - 1) },
        {static_cast<signed char>(x + 1), static_cast<signed char>(y - 1)},
    };

    for (int point = 0; point < 6; point++) {
        if (
            0 <= neighbors[point][0] && neighbors[point][0] < BOARD_SIZE &&
            0 <= neighbors[point][1] && neighbors[point][1] < BOARD_SIZE
        ) {
            output.push_back({neighbors[point][0], neighbors[point][1]});
        }
    }
}

/*************************************************
 * Methods for friends of HexState               *
 ************************************************/

size_t std::hash<HexState>::operator()(const GameState::HexState &state) {
    size_t result = 0;
    size_t pow_three = 1;
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            result += ((state.board)[x][y] + 1) * pow_three;
            pow_three *= 3;
        }
    }

    result += (state.whose_turn() * pow_three);
    return result;
}

std::ostream &GameState::operator<<(std::ostream &out, const GameState::HexState &state) {
    out << "╭";
    for (int t = 0; t < BOARD_SIZE; t++) {
        out << "──";
    }
    out << "╮\n";

    for (int y = BOARD_SIZE - 1; y >= 0; y--) {
        out << "│";
        for (int x = 0; x < BOARD_SIZE; x++) {
            switch (state.board[x][y]) {
                case PLAYER_NONE:
                    out << "  ";
                    break;
                case PLAYER_ONE:
                    out << "1 ";
                    break;
                case PLAYER_TWO:
                    out << "2 ";
                    break;
                default:
                    out << "?" << (int) state.board[x][y];
                    break;
            }
        }
        out << "│\n";
    }
    out << "╰";
    for (int t = 0; t < BOARD_SIZE; t++) {
        out << "──";
    }
    out << "╯\n";
    return out;
}

