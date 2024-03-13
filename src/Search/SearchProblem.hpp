#include <vector>

#include "../GameState/GameState.hpp"

namespace Search {
/**
* Node must have methods `bool is_goal()`, ``
*/
template<class Node>
class SearchProblem {
public:
    SearchProblem() = default;
    SearchProblem(SearchProblem &&) = delete;
    SearchProblem(const SearchProblem &) = delete;
    SearchProblem &operator=(SearchProblem &&) = delete;
    SearchProblem &operator=(const SearchProblem &) = delete;
    ~SearchProblem() = default;

    virtual std::vector<Node> *solve() const;
    virtual void solve(std::vector<Node> &buffer) const;
protected:
    Node initial;
};
}

