#include "canary.h"
#include <iostream>

void chirp(std::string s) {
    std::cerr << s << std::endl;
}

Canary::Canary() { chirp("ctor"); }
Canary::Canary(const Canary&) { chirp("copy"); }
Canary::Canary(Canary&&) { chirp("move"); }
Canary& Canary::operator=(const Canary&) { chirp("copy-assign");  return *this; }
Canary& Canary::operator=(Canary&&) { chirp("move-assign");  return *this; }
Canary::~Canary() { chirp("dtor"); }

static Canary instance;

const Canary& case1() { return instance; }
Canary case2() { return Canary{}; }
Canary&& case3() { return std::move(instance); }

