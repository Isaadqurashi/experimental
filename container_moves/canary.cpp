#include "canary.h"
#include <iostream>
using namespace std; 

void chirp(string s) {
    cerr << s << endl;
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
Canary&& case3() { return move(instance); }

