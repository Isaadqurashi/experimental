#include <cstdio>
#include <boost/hana.hpp>

template <typename T>
void print(T&& t) {
    printf(t.c_str());
}

void ok() {
    [](auto) {
        [](int) {
            auto str = BOOST_HANA_STRING("xxx");
            printf("%s\n", str.c_str());
        }(2);
    }(1);
}

void err() {
    [](auto) {
        [](auto) {
            auto str = BOOST_HANA_STRING("xxx");
            printf("%s\n", str.c_str());
        }(2);
    }(1);
}

int main() {
    ok();
    err();
    return 0;
}
