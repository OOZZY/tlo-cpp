#include <initializer_list>
#include <tlo-cpp/hash.hpp>
#include <tlo-cpp/test.hpp>

namespace {
template <class Combiner>
std::size_t combine(std::initializer_list<std::size_t> hashes) {
  Combiner combiner;

  for (std::size_t hash : hashes) {
    combiner.combineWith(hash);
  }

  return combiner.getHash();
}

TLO_TEST(JavaStyleHashCombiner) {
  using Combiner = tlo::JavaStyleHashCombiner;

  TLO_EXPECT_NE(combine<Combiner>({0}), 0U);

  TLO_EXPECT_NE(combine<Combiner>({1, 2, 3}), combine<Combiner>({2, 1, 3}));
  TLO_EXPECT_NE(combine<Combiner>({1, 2, 3}), combine<Combiner>({3, 2, 1}));
  TLO_EXPECT_NE(combine<Combiner>({2, 1, 3}), combine<Combiner>({3, 2, 1}));

  TLO_EXPECT_NE(combine<Combiner>({0}), combine<Combiner>({0, 0}));
  TLO_EXPECT_NE(combine<Combiner>({0}), combine<Combiner>({0, 0, 0}));
  TLO_EXPECT_NE(combine<Combiner>({0}), combine<Combiner>({0, 0, 0, 0}));

  TLO_EXPECT_NE(combine<Combiner>({0, 0}), combine<Combiner>({0, 0, 0}));
  TLO_EXPECT_NE(combine<Combiner>({0, 0}), combine<Combiner>({0, 0, 0, 0}));

  TLO_EXPECT_NE(combine<Combiner>({0, 0, 0}), combine<Combiner>({0, 0, 0, 0}));
}

TLO_TEST(BoostStyleHashCombiner) {
  using Combiner = tlo::BoostStyleHashCombiner;

  TLO_EXPECT_NE(combine<Combiner>({0}), 0U);

  TLO_EXPECT_NE(combine<Combiner>({1, 2, 3}), combine<Combiner>({2, 1, 3}));
  TLO_EXPECT_NE(combine<Combiner>({1, 2, 3}), combine<Combiner>({3, 2, 1}));
  TLO_EXPECT_NE(combine<Combiner>({2, 1, 3}), combine<Combiner>({3, 2, 1}));

  TLO_EXPECT_NE(combine<Combiner>({0}), combine<Combiner>({0, 0}));
  TLO_EXPECT_NE(combine<Combiner>({0}), combine<Combiner>({0, 0, 0}));
  TLO_EXPECT_NE(combine<Combiner>({0}), combine<Combiner>({0, 0, 0, 0}));

  TLO_EXPECT_NE(combine<Combiner>({0, 0}), combine<Combiner>({0, 0, 0}));
  TLO_EXPECT_NE(combine<Combiner>({0, 0}), combine<Combiner>({0, 0, 0, 0}));

  TLO_EXPECT_NE(combine<Combiner>({0, 0, 0}), combine<Combiner>({0, 0, 0, 0}));
}
}  // namespace
