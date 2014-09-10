#include <utility>
#include <memory>
#include <vector>

template <typename C, typename... Ts>
C MkCnt(Ts&&... vs) {
  C c;
  typename C::value_type t[] = {
     typename C::value_type(std::forward<Ts>(vs))...
  };
  std::copy(std::make_move_iterator(std::begin(t)),
            std::make_move_iterator(std::end(t)),
            std::inserter(c, c.end()));
  return c;
}

using OutC = std::vector<std::unique_ptr<int>>;

void test() {
  MkCnt<OutC>(new int{4}, new int{5}, new int{6});
}
