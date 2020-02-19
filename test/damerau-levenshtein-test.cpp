#include <string>
#include <tlo-cpp/damerau-levenshtein.hpp>
#include <tlo-cpp/test.hpp>

namespace {
using namespace std::string_literals;

TLO_TEST(damerLevenDistance1) {
  TLO_EXPECT(tlo::damerLevenDistance1(""s, ""s) == 0U);
  TLO_EXPECT(tlo::damerLevenDistance1("GAC"s, ""s) == 3U);
  TLO_EXPECT(tlo::damerLevenDistance1(""s, "AGCAT"s) == 5U);
  TLO_EXPECT(tlo::damerLevenDistance1("GAC"s, "AGCAT"s) == 3U);
  TLO_EXPECT(tlo::damerLevenDistance1("XMJYAUZ"s, "MZJAWXU"s) == 6U);
  TLO_EXPECT(tlo::damerLevenDistance1("___XMJYAUZ___"s, "__MZJAWXU___"s) == 7U);
  TLO_EXPECT(tlo::damerLevenDistance1("__XMJYAUZ___"s, "___MZJAWXU___-"s) ==
             7U);
  TLO_EXPECT(tlo::damerLevenDistance1("0123456789"s, "0123456789"s) == 0U);
  TLO_EXPECT(tlo::damerLevenDistance1("01234567"s, "abcdefghij"s) == 10U);
  TLO_EXPECT(tlo::damerLevenDistance1("aaabbb"s, "aaabbb___bbbccc"s) == 9U);
  TLO_EXPECT(tlo::damerLevenDistance1("bbbccc"s, "aaabbb___bbbccc"s) == 9U);
  TLO_EXPECT(tlo::damerLevenDistance1("aaabbbccc"s, "aaabbb___bbbccc"s) == 6U);
  TLO_EXPECT(tlo::damerLevenDistance1("sitting"s, "kitten"s) == 3U);
  TLO_EXPECT(tlo::damerLevenDistance1("Sunday"s, "Saturday"s) == 3U);
  TLO_EXPECT(tlo::damerLevenDistance1("CA"s, "ABC"s) == 2U);
}

TLO_TEST(damerLevenDistance2_with_damerLevenDistance1) {
  TLO_EXPECT(tlo::damerLevenDistance2(""s, ""s) == 0U);
  TLO_EXPECT(tlo::damerLevenDistance2("GAC"s, ""s) == 3U);
  TLO_EXPECT(tlo::damerLevenDistance2(""s, "AGCAT"s) == 5U);
  TLO_EXPECT(tlo::damerLevenDistance2("GAC"s, "AGCAT"s) == 3U);
  TLO_EXPECT(tlo::damerLevenDistance2("XMJYAUZ"s, "MZJAWXU"s) == 6U);
  TLO_EXPECT(tlo::damerLevenDistance2("___XMJYAUZ___"s, "__MZJAWXU___"s) == 7U);
  TLO_EXPECT(tlo::damerLevenDistance2("__XMJYAUZ___"s, "___MZJAWXU___-"s) ==
             7U);
  TLO_EXPECT(tlo::damerLevenDistance2("0123456789"s, "0123456789"s) == 0U);
  TLO_EXPECT(tlo::damerLevenDistance2("01234567"s, "abcdefghij"s) == 10U);
  TLO_EXPECT(tlo::damerLevenDistance2("aaabbb"s, "aaabbb___bbbccc"s) == 9U);
  TLO_EXPECT(tlo::damerLevenDistance2("bbbccc"s, "aaabbb___bbbccc"s) == 9U);
  TLO_EXPECT(tlo::damerLevenDistance2("aaabbbccc"s, "aaabbb___bbbccc"s) == 6U);
  TLO_EXPECT(tlo::damerLevenDistance2("sitting"s, "kitten"s) == 3U);
  TLO_EXPECT(tlo::damerLevenDistance2("Sunday"s, "Saturday"s) == 3U);
  TLO_EXPECT(tlo::damerLevenDistance2("CA"s, "ABC"s) == 2U);
}
}  // namespace
