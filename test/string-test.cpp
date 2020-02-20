#include <tlo-cpp/string.hpp>
#include <tlo-cpp/test.hpp>

namespace {
using StringVector = std::vector<std::string>;

TLO_TEST(split) {
  TLO_EXPECT(tlo::split("abc", ':') == StringVector({"abc"}));
  TLO_EXPECT(tlo::split("", ':') == StringVector({""}));

  TLO_EXPECT(tlo::split(":", ':') == StringVector({"", ""}));
  TLO_EXPECT(tlo::split(":b", ':') == StringVector({"", "b"}));
  TLO_EXPECT(tlo::split("a:", ':') == StringVector({"a", ""}));
  TLO_EXPECT(tlo::split("a:b", ':') == StringVector({"a", "b"}));

  TLO_EXPECT(tlo::split("::", ':') == StringVector({"", "", ""}));
  TLO_EXPECT(tlo::split("::c", ':') == StringVector({"", "", "c"}));
  TLO_EXPECT(tlo::split(":b:", ':') == StringVector({"", "b", ""}));
  TLO_EXPECT(tlo::split(":b:c", ':') == StringVector({"", "b", "c"}));
  TLO_EXPECT(tlo::split("a::", ':') == StringVector({"a", "", ""}));
  TLO_EXPECT(tlo::split("a::c", ':') == StringVector({"a", "", "c"}));
  TLO_EXPECT(tlo::split("a:b:", ':') == StringVector({"a", "b", ""}));
  TLO_EXPECT(tlo::split("a:b:c", ':') == StringVector({"a", "b", "c"}));
}

TLO_TEST(toString) {
  TLO_EXPECT_EQ(tlo::toString(42), "42");
  TLO_EXPECT_EQ(tlo::toString(42.0), "42");
  TLO_EXPECT_EQ(tlo::toString("42"), "42");
}

TLO_TEST(join) {
  TLO_EXPECT_EQ(tlo::join(0, "?", ", "), "");
  TLO_EXPECT_EQ(tlo::join(1, "?", ", "), "?");
  TLO_EXPECT_EQ(tlo::join(2, "?", ", "), "?, ?");
  TLO_EXPECT_EQ(tlo::join(3, "?", ", "), "?, ?, ?");

  TLO_EXPECT_EQ(tlo::join(0, "", ", "), "");
  TLO_EXPECT_EQ(tlo::join(1, "", ", "), "");
  TLO_EXPECT_EQ(tlo::join(2, "", ", "), ", ");
  TLO_EXPECT_EQ(tlo::join(3, "", ", "), ", , ");

  TLO_EXPECT_EQ(tlo::join(0, "?", ""), "");
  TLO_EXPECT_EQ(tlo::join(1, "?", ""), "?");
  TLO_EXPECT_EQ(tlo::join(2, "?", ""), "??");
  TLO_EXPECT_EQ(tlo::join(3, "?", ""), "???");
}
}  // namespace
