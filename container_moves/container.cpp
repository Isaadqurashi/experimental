#include "container.h"
#include "canary.h"

template<> void ExternContainer<Canary>::byValue(Canary c) { v = std::move(c); }
template<> void ExternContainer<Canary>::byRef(const Canary& c) { v = std::move(c); }
template<> void ExternContainer<Canary>::byRef(Canary&& c) { v = std::move(c); }
