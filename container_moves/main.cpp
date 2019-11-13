
#include <iostream>
#include "container.h"
#include "canary.h"

static void barrier() { asm volatile("\nnop\nnop\nnop\n"); }

template <typename C>
void byValueTest(C& c) {
    std::cerr << ">>> byValueTest" << std::endl;
    c.byValue(case1());
    barrier();
    c.byValue(case2());
    barrier();
    c.byValue(case3());
    std::cerr << "<<< byValueTest" << std::endl;
}

template <typename C>
void byRefTest(C& c) {
    std::cerr << ">>> byRefTest" << std::endl;
    c.byRef(case1());
    barrier();
    c.byRef(case2());
    barrier();
    c.byRef(case3());
    std::cerr << "<<< byRefTest" << std::endl;
}

void TestInlinedContainer() {
    std::cerr << ">>> Inlined Container Test" << std::endl;
    InlineContainer<Canary> container;
    byValueTest(container);
    byRefTest(container);
    std::cerr << "<<< Inlined Container Test" << std::endl;
}

void TestExternContainer() {
    std::cerr << ">>> Extern Container Test" << std::endl;
    ExternContainer<Canary> container;
    byValueTest(container);
    byRefTest(container);
    std::cerr << "<<< Extern Container Test" << std::endl;
}

int main() {
    TestInlinedContainer();
    TestExternContainer();
    return 0;
}
