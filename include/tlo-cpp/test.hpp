#ifndef TLO_CPP_TEST_HPP
#define TLO_CPP_TEST_HPP

#include <deque>

namespace tlo {
namespace test {
// base class for a unit test, a test fixture, or a unit test using a test
// fixture
struct Test {
  virtual const char *testName() const = 0;
  virtual void run() const = 0;
  virtual ~Test();
};

namespace internal {
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
bool expect(bool isExpect, bool condition, const char *file, int line,
            const char *func, const char *conditionString);
}  // namespace internal

#define TLO_EXPECT(condition)                                          \
  ::tlo::test::internal::expect(true, (condition), __FILE__, __LINE__, \
                                __func__, #condition)

#define TLO_ASSERT(condition)                                                  \
  do {                                                                         \
    if (!::tlo::test::internal::expect(false, (condition), __FILE__, __LINE__, \
                                       __func__, #condition)) {                \
      return;                                                                  \
    }                                                                          \
  } while (0)

void runTests();
void printReport();
[[noreturn]] void exit();
}  // namespace test
}  // namespace tlo

#endif  // TLO_CPP_TEST_HPP
