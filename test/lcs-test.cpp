#include <string>
#include <tlo-cpp/lcs.hpp>
#include <tlo-cpp/test.hpp>

namespace {
using namespace std::string_literals;

tlo::LcsLengthResult result(std::size_t lcsLength, std::size_t lcsDistance) {
  return {lcsLength, lcsDistance};
}

TLO_TEST(lcsLength1) {
  TLO_EXPECT_EQ(tlo::lcsLength1(""s, ""s), result(0, 0));
  TLO_EXPECT_EQ(tlo::lcsLength1("GAC"s, ""s), result(0, 3));
  TLO_EXPECT_EQ(tlo::lcsLength1(""s, "AGCAT"s), result(0, 5));
  TLO_EXPECT_EQ(tlo::lcsLength1("GAC"s, "AGCAT"s), result(2, 4));
  TLO_EXPECT_EQ(tlo::lcsLength1("XMJYAUZ"s, "MZJAWXU"s), result(4, 6));
  TLO_EXPECT_EQ(tlo::lcsLength1("___XMJYAUZ___"s, "__MZJAWXU___"s),
                result(9, 7));
  TLO_EXPECT_EQ(tlo::lcsLength1("__XMJYAUZ___"s, "___MZJAWXU___-"s),
                result(9, 8));
  TLO_EXPECT_EQ(tlo::lcsLength1("0123456789"s, "0123456789"s), result(10, 0));
  TLO_EXPECT_EQ(tlo::lcsLength1("01234567"s, "abcdefghij"s), result(0, 18));
  TLO_EXPECT_EQ(tlo::lcsLength1("aaabbb"s, "aaabbb___bbbccc"s), result(6, 9));
  TLO_EXPECT_EQ(tlo::lcsLength1("bbbccc"s, "aaabbb___bbbccc"s), result(6, 9));
  TLO_EXPECT_EQ(tlo::lcsLength1("aaabbbccc"s, "aaabbb___bbbccc"s),
                result(9, 6));
  TLO_EXPECT_EQ(tlo::lcsLength1("sitting"s, "kitten"s), result(4, 5));
  TLO_EXPECT_EQ(tlo::lcsLength1("Sunday"s, "Saturday"s), result(5, 4));
  TLO_EXPECT_EQ(tlo::lcsLength1("CA"s, "ABC"s), result(1, 3));
}

TLO_TEST(lcsLength2) {
  TLO_EXPECT_EQ(tlo::lcsLength2(""s, ""s), result(0, 0));
  TLO_EXPECT_EQ(tlo::lcsLength2("GAC"s, ""s), result(0, 3));
  TLO_EXPECT_EQ(tlo::lcsLength2(""s, "AGCAT"s), result(0, 5));
  TLO_EXPECT_EQ(tlo::lcsLength2("GAC"s, "AGCAT"s), result(2, 4));
  TLO_EXPECT_EQ(tlo::lcsLength2("XMJYAUZ"s, "MZJAWXU"s), result(4, 6));
  TLO_EXPECT_EQ(tlo::lcsLength2("___XMJYAUZ___"s, "__MZJAWXU___"s),
                result(9, 7));
  TLO_EXPECT_EQ(tlo::lcsLength2("__XMJYAUZ___"s, "___MZJAWXU___-"s),
                result(9, 8));
  TLO_EXPECT_EQ(tlo::lcsLength2("0123456789"s, "0123456789"s), result(10, 0));
  TLO_EXPECT_EQ(tlo::lcsLength2("01234567"s, "abcdefghij"s), result(0, 18));
  TLO_EXPECT_EQ(tlo::lcsLength2("aaabbb"s, "aaabbb___bbbccc"s), result(6, 9));
  TLO_EXPECT_EQ(tlo::lcsLength2("bbbccc"s, "aaabbb___bbbccc"s), result(6, 9));
  TLO_EXPECT_EQ(tlo::lcsLength2("aaabbbccc"s, "aaabbb___bbbccc"s),
                result(9, 6));
  TLO_EXPECT_EQ(tlo::lcsLength2("sitting"s, "kitten"s), result(4, 5));
  TLO_EXPECT_EQ(tlo::lcsLength2("Sunday"s, "Saturday"s), result(5, 4));
  TLO_EXPECT_EQ(tlo::lcsLength2("CA"s, "ABC"s), result(1, 3));
}

TLO_TEST(lcsLength3_with_lcsLength1) {
  const auto &lcsl1_ = tlo::lcsLength1_<std::string>;

  TLO_EXPECT_EQ(tlo::lcsLength3(""s, ""s, lcsl1_), result(0, 0));
  TLO_EXPECT_EQ(tlo::lcsLength3("GAC"s, ""s, lcsl1_), result(0, 3));
  TLO_EXPECT_EQ(tlo::lcsLength3(""s, "AGCAT"s, lcsl1_), result(0, 5));
  TLO_EXPECT_EQ(tlo::lcsLength3("GAC"s, "AGCAT"s, lcsl1_), result(2, 4));
  TLO_EXPECT_EQ(tlo::lcsLength3("XMJYAUZ"s, "MZJAWXU"s, lcsl1_), result(4, 6));
  TLO_EXPECT_EQ(tlo::lcsLength3("___XMJYAUZ___"s, "__MZJAWXU___"s, lcsl1_),
                result(9, 7));
  TLO_EXPECT_EQ(tlo::lcsLength3("__XMJYAUZ___"s, "___MZJAWXU___-"s, lcsl1_),
                result(9, 8));
  TLO_EXPECT_EQ(tlo::lcsLength3("0123456789"s, "0123456789"s, lcsl1_),
                result(10, 0));
  TLO_EXPECT_EQ(tlo::lcsLength3("01234567"s, "abcdefghij"s, lcsl1_),
                result(0, 18));
  TLO_EXPECT_EQ(tlo::lcsLength3("aaabbb"s, "aaabbb___bbbccc"s, lcsl1_),
                result(6, 9));
  TLO_EXPECT_EQ(tlo::lcsLength3("bbbccc"s, "aaabbb___bbbccc"s, lcsl1_),
                result(6, 9));
  TLO_EXPECT_EQ(tlo::lcsLength3("aaabbbccc"s, "aaabbb___bbbccc"s, lcsl1_),
                result(9, 6));
  TLO_EXPECT_EQ(tlo::lcsLength3("sitting"s, "kitten"s, lcsl1_), result(4, 5));
  TLO_EXPECT_EQ(tlo::lcsLength3("Sunday"s, "Saturday"s, lcsl1_), result(5, 4));
  TLO_EXPECT_EQ(tlo::lcsLength3("CA"s, "ABC"s, lcsl1_), result(1, 3));
}

TLO_TEST(lcsLength3_with_lcsLength2) {
  TLO_EXPECT_EQ(tlo::lcsLength3(""s, ""s), result(0, 0));
  TLO_EXPECT_EQ(tlo::lcsLength3("GAC"s, ""s), result(0, 3));
  TLO_EXPECT_EQ(tlo::lcsLength3(""s, "AGCAT"s), result(0, 5));
  TLO_EXPECT_EQ(tlo::lcsLength3("GAC"s, "AGCAT"s), result(2, 4));
  TLO_EXPECT_EQ(tlo::lcsLength3("XMJYAUZ"s, "MZJAWXU"s), result(4, 6));
  TLO_EXPECT_EQ(tlo::lcsLength3("___XMJYAUZ___"s, "__MZJAWXU___"s),
                result(9, 7));
  TLO_EXPECT_EQ(tlo::lcsLength3("__XMJYAUZ___"s, "___MZJAWXU___-"s),
                result(9, 8));
  TLO_EXPECT_EQ(tlo::lcsLength3("0123456789"s, "0123456789"s), result(10, 0));
  TLO_EXPECT_EQ(tlo::lcsLength3("01234567"s, "abcdefghij"s), result(0, 18));
  TLO_EXPECT_EQ(tlo::lcsLength3("aaabbb"s, "aaabbb___bbbccc"s), result(6, 9));
  TLO_EXPECT_EQ(tlo::lcsLength3("bbbccc"s, "aaabbb___bbbccc"s), result(6, 9));
  TLO_EXPECT_EQ(tlo::lcsLength3("aaabbbccc"s, "aaabbb___bbbccc"s),
                result(9, 6));
  TLO_EXPECT_EQ(tlo::lcsLength3("sitting"s, "kitten"s), result(4, 5));
  TLO_EXPECT_EQ(tlo::lcsLength3("Sunday"s, "Saturday"s), result(5, 4));
  TLO_EXPECT_EQ(tlo::lcsLength3("CA"s, "ABC"s), result(1, 3));
}
}  // namespace
