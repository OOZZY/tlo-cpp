#include <map>
#include <tlo-cpp/container.hpp>
#include <tlo-cpp/test.hpp>
#include <unordered_set>
#include <vector>

namespace {
const std::map<int, int> map{{1, 10}, {2, 20}, {3, 30}};

TLO_TEST(getKeySequence) {
  TLO_EXPECT(tlo::getKeySequence<std::vector<int>>(map) ==
             (std::vector<int>{1, 2, 3}));
}

TLO_TEST(getValueSequence) {
  TLO_EXPECT(tlo::getValueSequence<std::vector<int>>(map) ==
             (std::vector<int>{10, 20, 30}));
}

TLO_TEST(getKeySet) {
  TLO_EXPECT(tlo::getKeySet<std::unordered_set<int>>(map) ==
             (std::unordered_set<int>{1, 2, 3}));
}

TLO_TEST(getValueSet) {
  TLO_EXPECT(tlo::getValueSet<std::unordered_set<int>>(map) ==
             (std::unordered_set<int>{10, 20, 30}));
}
}  // namespace
