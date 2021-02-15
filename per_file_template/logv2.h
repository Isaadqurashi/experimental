
#include <array>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <utility>

#pragma once

namespace mongo::logv2 {

/** Like boost::hana::string. */
template <char... Cs> struct StringPack {
  static constexpr const char storage[] = {Cs..., '\0'};
};

template <typename S, size_t... Is>
constexpr auto prepare(S, std::index_sequence<Is...>) {
  return StringPack<S::get()[Is]...>{};
}
template <typename S> constexpr decltype(auto) prepare(S s) {
  return prepare(s, std::make_index_sequence<sizeof(S::get()) - 1>{});
}

enum LogComponent : int {
    kDefault = -1,
    kTest,
    kMain,
};
inline std::ostream& operator<<(std::ostream& os, LogComponent lc) {
    switch (lc) {
        case kDefault:
            return os << "kDefault";
        case kTest:
            return os << "kTest";
        case kMain:
            return os << "kMain";
        default:
            return os << "???" << int(lc);
    }
}

template <typename FileName>
inline LogComponent logComponent = LogComponent::kDefault;

#define LOGV2_STRING_PACK(s)                                                   \
  ::mongo::logv2::prepare([] {                                                 \
    struct Tmp {                                                               \
      static constexpr decltype(auto) get() { return s; }                      \
    };                                                                         \
    return Tmp{};                                                              \
  }())

#define LOGV2_COMPONENT_FOR_FILE                                               \
  []() -> auto & {                                                             \
    struct Tmp {                                                               \
      static auto get() { return LOGV2_STRING_PACK(__FILE__); }                \
    };                                                                         \
    return ::mongo::logv2::logComponent<decltype(Tmp::get())>;                 \
  }                                                                            \
  ()

} // namespace mongo::logv2

#define LOG(msg) std::cout << LOGV2_COMPONENT_FOR_FILE << ":" << msg << "\n";

#define DEFINE_LOG_COMPONENT_FOR_FILE(value)                                   \
  namespace {                                                                  \
  template <int> class Registrant;                                            \
  template <> class Registrant<__COUNTER__> {                                  \
    static inline const auto _dummy = [] {                                     \
      LOGV2_COMPONENT_FOR_FILE = ::mongo::logv2::LogComponent{value};          \
      LOG("initialized " __FILE__);                                            \
      return 0;                                                                \
    }();                                                                       \
  };                                                                           \
  }

DEFINE_LOG_COMPONENT_FOR_FILE(mongo::logv2::LogComponent::kTest)

inline int inlineFunc() {
  LOG("from inlineFunc");
  return 0;
}
