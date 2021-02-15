
#include <iostream>
#include <utility>

template <char...Cs>
struct String {};

template <typename S, size_t...Is>
constexpr auto prepare_(S s, std::index_sequence<Is...>) {
    return String<S::get()[Is]...>{};
}

template <typename S>
auto prepare(S s) {
    std::cout << "[" << __PRETTY_FUNCTION__ << "]" << std::endl;
    return prepare_(s, std::make_index_sequence<sizeof(S::get())-1>{});
}

/*
 * Two LOGV2 statements in the Numeric test below showed unexplained build failures.
 * Must be related to the nested nature of the lambdas.
 */
void TestBugWithStringPackEvaluation() {
    auto outer = [](auto a) {
        auto inner = [](auto b) {
            auto str = prepare([]() -> decltype(auto) {
                struct Tmp {
                    static constexpr decltype(auto) get() { return "hi"; }
                };
                return Tmp{};
            }());
        };
        inner(111);
    };
    outer(222);
}


int main() {
    TestBugWithStringPackEvaluation();
    return 0;
}
