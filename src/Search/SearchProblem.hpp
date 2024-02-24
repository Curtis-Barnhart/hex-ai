#include <vector>

#include "../GameState/GameState.hpp"

namespace Search {
template<class A, typename G>
class SearchProblem {
public:
    SearchProblem() = default;
    SearchProblem(SearchProblem &&) = default;
    SearchProblem(const SearchProblem &) = default;
    SearchProblem &operator=(SearchProblem &&) = default;
    SearchProblem &operator=(const SearchProblem &) = default;
    ~SearchProblem() = default;

    virtual std::vector<A> *solve() const;
    virtual void solve(std::vector<A> &buffer) const;
protected:
    GameState::State<A> initial;
    G goal_test;
};

template<class A, typename G>
class DFSProblem: public Search::SearchProblem<A, G> {
public:
    DFSProblem() = default;
    DFSProblem(DFSProblem &&) = default;
    DFSProblem(const DFSProblem &) = default;
    DFSProblem &operator=(DFSProblem &&) = default;
    DFSProblem &operator=(const DFSProblem &) = default;
    ~DFSProblem() = default;

    std::vector<A> *solve() const override;
    void solve(std::vector<A> &buffer) const override {

    }
};
}
