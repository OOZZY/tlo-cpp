#include <string>
#include <tlo-cpp/levenshtein.hpp>
#include <tlo-cpp/test.hpp>

namespace {
template <class T1, class T2>
bool equal(const T1 &obj1, const T2 &obj2) {
  return obj1 == obj2;
}

using namespace std::string_literals;

TLO_TEST(levenshteinDistance1) {
  TLO_EXPECT(equal(tlo::levenshteinDistance1(""s, ""s), 0U));
  TLO_EXPECT(equal(tlo::levenshteinDistance1("GAC"s, ""s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance1(""s, "AGCAT"s), 5U));
  TLO_EXPECT(equal(tlo::levenshteinDistance1("GAC"s, "AGCAT"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance1("XMJYAUZ"s, "MZJAWXU"s), 6U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance1("___XMJYAUZ___"s, "__MZJAWXU___"s), 7U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance1("__XMJYAUZ___"s, "___MZJAWXU___-"s), 7U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance1("0123456789"s, "0123456789"s), 0U));
  TLO_EXPECT(equal(tlo::levenshteinDistance1("01234567"s, "abcdefghij"s), 10U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance1("aaabbb"s, "aaabbb___bbbccc"s), 9U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance1("bbbccc"s, "aaabbb___bbbccc"s), 9U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance1("aaabbbccc"s, "aaabbb___bbbccc"s), 6U));
  TLO_EXPECT(equal(tlo::levenshteinDistance1("sitting"s, "kitten"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance1("Sunday"s, "Saturday"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance1("CA"s, "ABC"s), 3U));
}

TLO_TEST(levenshteinDistance2) {
  TLO_EXPECT(equal(tlo::levenshteinDistance2(""s, ""s), 0U));
  TLO_EXPECT(equal(tlo::levenshteinDistance2("GAC"s, ""s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance2(""s, "AGCAT"s), 5U));
  TLO_EXPECT(equal(tlo::levenshteinDistance2("GAC"s, "AGCAT"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance2("XMJYAUZ"s, "MZJAWXU"s), 6U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance2("___XMJYAUZ___"s, "__MZJAWXU___"s), 7U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance2("__XMJYAUZ___"s, "___MZJAWXU___-"s), 7U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance2("0123456789"s, "0123456789"s), 0U));
  TLO_EXPECT(equal(tlo::levenshteinDistance2("01234567"s, "abcdefghij"s), 10U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance2("aaabbb"s, "aaabbb___bbbccc"s), 9U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance2("bbbccc"s, "aaabbb___bbbccc"s), 9U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance2("aaabbbccc"s, "aaabbb___bbbccc"s), 6U));
  TLO_EXPECT(equal(tlo::levenshteinDistance2("sitting"s, "kitten"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance2("Sunday"s, "Saturday"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance2("CA"s, "ABC"s), 3U));
}

TLO_TEST(levenshteinDistance3_with_levenshteinDistance1) {
  const auto &ld1_ = tlo::levenshteinDistance1_<std::string>;

  TLO_EXPECT(equal(tlo::levenshteinDistance3(""s, ""s, ld1_), 0U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("GAC"s, ""s, ld1_), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3(""s, "AGCAT"s, ld1_), 5U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("GAC"s, "AGCAT"s, ld1_), 3U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("XMJYAUZ"s, "MZJAWXU"s, ld1_), 6U));
  TLO_EXPECT(equal(
      tlo::levenshteinDistance3("___XMJYAUZ___"s, "__MZJAWXU___"s, ld1_), 7U));
  TLO_EXPECT(equal(
      tlo::levenshteinDistance3("__XMJYAUZ___"s, "___MZJAWXU___-"s, ld1_), 7U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("0123456789"s, "0123456789"s, ld1_), 0U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("01234567"s, "abcdefghij"s, ld1_), 10U));
  TLO_EXPECT(equal(
      tlo::levenshteinDistance3("aaabbb"s, "aaabbb___bbbccc"s, ld1_), 9U));
  TLO_EXPECT(equal(
      tlo::levenshteinDistance3("bbbccc"s, "aaabbb___bbbccc"s, ld1_), 9U));
  TLO_EXPECT(equal(
      tlo::levenshteinDistance3("aaabbbccc"s, "aaabbb___bbbccc"s, ld1_), 6U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("sitting"s, "kitten"s, ld1_), 3U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("Sunday"s, "Saturday"s, ld1_), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("CA"s, "ABC"s, ld1_), 3U));
}

TLO_TEST(levenshteinDistance3_with_levenshteinDistance2) {
  TLO_EXPECT(equal(tlo::levenshteinDistance3(""s, ""s), 0U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("GAC"s, ""s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3(""s, "AGCAT"s), 5U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("GAC"s, "AGCAT"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("XMJYAUZ"s, "MZJAWXU"s), 6U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("___XMJYAUZ___"s, "__MZJAWXU___"s), 7U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("__XMJYAUZ___"s, "___MZJAWXU___-"s), 7U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("0123456789"s, "0123456789"s), 0U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("01234567"s, "abcdefghij"s), 10U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("aaabbb"s, "aaabbb___bbbccc"s), 9U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("bbbccc"s, "aaabbb___bbbccc"s), 9U));
  TLO_EXPECT(
      equal(tlo::levenshteinDistance3("aaabbbccc"s, "aaabbb___bbbccc"s), 6U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("sitting"s, "kitten"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("Sunday"s, "Saturday"s), 3U));
  TLO_EXPECT(equal(tlo::levenshteinDistance3("CA"s, "ABC"s), 3U));
}
}  // namespace
