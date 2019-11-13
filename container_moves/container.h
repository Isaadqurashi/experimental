#pragma once
#include <utility>
#include "canary.h"

template <typename T>
struct InlineContainer {
    void byValue(T c) { v = std::move(c); }
    void byRef(const T& c) { v = c; }
    void byRef(T&& c) { v = std::move(c); }
    T v;
};

template <typename T>
struct ExternContainer {
    void byValue(T c);
    void byRef(const T& c);
    void byRef(T&& c);

    T v;
};

template<> void ExternContainer<Canary>::byValue(Canary c);
template<> void ExternContainer<Canary>::byRef(const Canary& c);
template<> void ExternContainer<Canary>::byRef(Canary&& c);
