#include <cstdio>
#include <boost/hana.hpp>

template <typename T>
void print(T&& t) {
    printf(t.c_str());
}

#ifdef SHOW_ERROR
#define INNER_ARG auto
#else
#define INNER_ARG int
#endif

int main() {
    [](auto) {
        [](INNER_ARG) {
            auto str = BOOST_HANA_STRING("xxx");
            printf("%s\n", str.c_str());
        }(2);
    }(1);
    return 0;
}
