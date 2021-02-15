
#include <iostream>
#include <utility>

template <typename T>
void dump(const T& t) {
    std::cout << "[" << __PRETTY_FUNCTION__ << "] t=" << t.c_str() << std::endl;
}

template <char...Cs>
struct String {
    static constexpr const char storage[] = {Cs..., 0};
    constexpr const char* c_str() const { return storage; }
};

template <typename S, size_t...Is>
constexpr auto prepare_(S s, std::index_sequence<Is...>) {
    return String<S::get()[Is]...>{};
}

template <typename S>
auto prepare(S s) {
    std::cout << "[" << __PRETTY_FUNCTION__ << "]" << std::endl;
    return prepare_(s, std::make_index_sequence<sizeof(S::get())-1>{});
}

#define MACRO(s) ( \
    prepare([]() -> decltype(auto) { \
        struct Tmp { \
            static constexpr decltype(auto) get() { return s; } \
        }; \
        return Tmp{}; \
    }()))


/*
 * Two LOGV2 statements in the Numeric test below showed unexplained build failures.
 * Must be related to the nested nature of the lambdas.
 */
void TestBugWithStringPackEvaluation() {
    [[maybe_unused]] auto outer = [](auto a) {
        auto inner = [](auto b) {
            auto str = MACRO("hi");
            std::cout << str.c_str() << "\n";
        };
        inner(a);
    };
    outer(222);
}


int main() {
    TestBugWithStringPackEvaluation();
    return 0;
}
