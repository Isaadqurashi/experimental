
template <typename T>
void print(T&&) {}

void TestBugWithStringPackEvaluation() {
    [](auto) {
        [](auto) {
            print([]() -> decltype(auto) {
                struct Tmp {
                    static constexpr decltype(auto) get() { return 123; }
                };
                return Tmp{};
            }());
        }(2);
    }(1);
}


int main() {
    TestBugWithStringPackEvaluation();
    return 0;
}
