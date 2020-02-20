#include <tlo-cpp/stop.hpp>
#include <tlo-cpp/test.hpp>

namespace {
TLO_TEST(tloRequestStop) {
  TLO_EXPECT_FALSE(tlo::stopRequested.load());

  tloRequestStop(int());

  TLO_EXPECT(tlo::stopRequested.load());
}
}  // namespace
