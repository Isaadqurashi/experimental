
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

#define LOG_IMPL() \
       dump(MACRO(__FILE__)) \
       /**/

int main() {
    //auto s = MACRO("hello");
    //std::cout << s << "\n";
    LOG_IMPL();
    return 0;
}
