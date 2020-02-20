#ifndef TLO_CPP_TEST_HPP
#define TLO_CPP_TEST_HPP

#include <deque>
#include <iostream>

namespace tlo {
namespace test {
// Base class for a unit test, a test fixture, or a unit test using a test
// fixture.
struct Test {
  virtual const char *testName() const = 0;
  virtual void run() const = 0;
  virtual ~Test();
};

namespace internal {
// Constructs the deque on first use of function.
std::deque<const Test *> &getTests();
}  // namespace internal

#define TLO_TEST(TestName)                                      \
  struct GeneratedTest##TestName : ::tlo::test::Test {          \
    GeneratedTest##TestName() {                                 \
      ::tlo::test::internal::getTests().push_back(this);        \
    }                                                           \
    const char *testName() const override { return #TestName; } \
    void run() const override;                                  \
  };                                                            \
  const GeneratedTest##TestName generatedTest##TestName;        \
  void GeneratedTest##TestName::run() const

#define TLO_TEST_USING_FIXTURE(FixtureName, TestName)         \
  struct GeneratedTest##FixtureName##TestName : FixtureName { \
    GeneratedTest##FixtureName##TestName() {                  \
      ::tlo::test::internal::getTests().push_back(this);      \
    }                                                         \
    const char *testName() const override {                   \
      return #FixtureName "." #TestName;                      \
    }                                                         \
    void run() const override;                                \
  };                                                          \
  const GeneratedTest##FixtureName##TestName                  \
      generatedTest##FixtureName##TestName;                   \
  void GeneratedTest##FixtureName##TestName::run() const

namespace internal {
extern int numExpects;
extern int numAsserts;
extern int numExpectsFailed;
extern int numAssertsFailed;
extern int numObjectsCaught;

// Returns condition.
bool expect(bool isExpect, bool condition, const char *conditionString,
            const char *file, int line, const char *testName);
}  // namespace internal

#define TLO_EXPECT(condition)                                            \
  ::tlo::test::internal::expect(true, (condition), #condition, __FILE__, \
                                __LINE__, testName())

#define TLO_ASSERT(condition)                                             \
  do {                                                                    \
    if (!::tlo::test::internal::expect(false, (condition), #condition,    \
                                       __FILE__, __LINE__, testName())) { \
      return;                                                             \
    }                                                                     \
  } while (0)

namespace internal {
// Returns condition.
template <class Left, class Right>
bool test(bool isExpect, const char *testType, bool condition, const Left &left,
          const char *leftString, const Right &right, const char *rightString,
          const char *file, int line, const char *testName) {
  if (isExpect) {
    ++numExpects;
  } else {
    ++numAsserts;
  }

  if (!condition) {
    std::cout << file << ":" << line << ": " << testName << ": ";

    if (isExpect) {
      ++numExpectsFailed;

      std::cout << "TLO_EXPECT_";
    } else {
      ++numAssertsFailed;

      std::cout << "TLO_ASSERT_";
    }

    std::cout << testType << " failed:" << std::endl;
    std::cout << "Left Expression : " << leftString << std::endl;
    std::cout << "Left Value      : " << left << std::endl;
    std::cout << "Right Expression: " << rightString << std::endl;
    std::cout << "Right Value     : " << right << std::endl;
  }

  return condition;
}

// Returns result of comparison.
template <class Left, class Right>
bool expectEqual(bool isExpect, const Left &left, const char *leftString,
                 const Right &right, const char *rightString, const char *file,
                 int line, const char *testName) {
  return test(isExpect, "EQ", left == right, left, leftString, right,
              rightString, file, line, testName);
}

// Returns result of comparison.
template <class Left, class Right>
bool expectNotEqual(bool isExpect, const Left &left, const char *leftString,
                    const Right &right, const char *rightString,
                    const char *file, int line, const char *testName) {
  return test(isExpect, "NE", left != right, left, leftString, right,
              rightString, file, line, testName);
}

// Returns result of comparison.
template <class Left, class Right>
bool expectLessThan(bool isExpect, const Left &left, const char *leftString,
                    const Right &right, const char *rightString,
                    const char *file, int line, const char *testName) {
  return test(isExpect, "LT", left < right, left, leftString, right,
              rightString, file, line, testName);
}

// Returns result of comparison.
template <class Left, class Right>
bool expectLessThanOrEqual(bool isExpect, const Left &left,
                           const char *leftString, const Right &right,
                           const char *rightString, const char *file, int line,
                           const char *testName) {
  return test(isExpect, "LE", left <= right, left, leftString, right,
              rightString, file, line, testName);
}

// Returns result of comparison.
template <class Left, class Right>
bool expectGreaterThan(bool isExpect, const Left &left, const char *leftString,
                       const Right &right, const char *rightString,
                       const char *file, int line, const char *testName) {
  return test(isExpect, "GT", left > right, left, leftString, right,
              rightString, file, line, testName);
}

// Returns result of comparison.
template <class Left, class Right>
bool expectGreaterThanOrEqual(bool isExpect, const Left &left,
                              const char *leftString, const Right &right,
                              const char *rightString, const char *file,
                              int line, const char *testName) {
  return test(isExpect, "GE", left >= right, left, leftString, right,
              rightString, file, line, testName);
}
}  // namespace internal

#define TLO_EXPECT_EQ(left, right)                                         \
  ::tlo::test::internal::expectEqual(true, (left), #left, (right), #right, \
                                     __FILE__, __LINE__, testName())

#define TLO_ASSERT_EQ(left, right)                                         \
  do {                                                                     \
    if (!::tlo::test::internal::expectEqual(false, (left), #left, (right), \
                                            #right, __FILE__, __LINE__,    \
                                            testName())) {                 \
      return;                                                              \
    }                                                                      \
  } while (0)

#define TLO_EXPECT_NE(left, right)                                            \
  ::tlo::test::internal::expectNotEqual(true, (left), #left, (right), #right, \
                                        __FILE__, __LINE__, testName())

#define TLO_ASSERT_NE(left, right)                                            \
  do {                                                                        \
    if (!::tlo::test::internal::expectNotEqual(false, (left), #left, (right), \
                                               #right, __FILE__, __LINE__,    \
                                               testName())) {                 \
      return;                                                                 \
    }                                                                         \
  } while (0)

#define TLO_EXPECT_LT(left, right)                                            \
  ::tlo::test::internal::expectLessThan(true, (left), #left, (right), #right, \
                                        __FILE__, __LINE__, testName())

#define TLO_ASSERT_LT(left, right)                                            \
  do {                                                                        \
    if (!::tlo::test::internal::expectLessThan(false, (left), #left, (right), \
                                               #right, __FILE__, __LINE__,    \
                                               testName())) {                 \
      return;                                                                 \
    }                                                                         \
  } while (0)

#define TLO_EXPECT_LE(left, right)              \
  ::tlo::test::internal::expectLessThanOrEqual( \
      true, (left), #left, (right), #right, __FILE__, __LINE__, testName())

#define TLO_ASSERT_LE(left, right)                                     \
  do {                                                                 \
    if (!::tlo::test::internal::expectLessThanOrEqual(                 \
            false, (left), #left, (right), #right, __FILE__, __LINE__, \
            testName())) {                                             \
      return;                                                          \
    }                                                                  \
  } while (0)

#define TLO_EXPECT_GT(left, right)          \
  ::tlo::test::internal::expectGreaterThan( \
      true, (left), #left, (right), #right, __FILE__, __LINE__, testName())

#define TLO_ASSERT_GT(left, right)                                           \
  do {                                                                       \
    if (!::tlo::test::internal::expectGreaterThan(false, (left), #left,      \
                                                  (right), #right, __FILE__, \
                                                  __LINE__, testName())) {   \
      return;                                                                \
    }                                                                        \
  } while (0)

#define TLO_EXPECT_GE(left, right)                 \
  ::tlo::test::internal::expectGreaterThanOrEqual( \
      true, (left), #left, (right), #right, __FILE__, __LINE__, testName())

#define TLO_ASSERT_GE(left, right)                                     \
  do {                                                                 \
    if (!::tlo::test::internal::expectGreaterThanOrEqual(              \
            false, (left), #left, (right), #right, __FILE__, __LINE__, \
            testName())) {                                             \
      return;                                                          \
    }                                                                  \
  } while (0)

void runTests();
void printReport();
[[noreturn]] void exit();
}  // namespace test
}  // namespace tlo

#endif  // TLO_CPP_TEST_HPP
