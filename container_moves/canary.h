
#pragma once

#include <utility>

struct Canary {
    Canary();
    Canary(const Canary&);
    Canary(Canary&&);
    Canary& operator=(const Canary&);
    Canary& operator=(Canary&&);
    ~Canary();
};

const Canary& case1();  // case 1: lvalue
Canary case2();  // case 2: temporary
Canary&& case3();  // case 3: moved lvalue

