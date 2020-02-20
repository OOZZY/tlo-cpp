#include "tlo-cpp/test.hpp"

namespace tlo {
namespace test {
Test::~Test() = default;

namespace internal {
std::deque<const Test *> &getTests() {
  static std::deque<const Test *> tests;
  return tests;
}

int numExpects = 0;
int numAsserts = 0;
int numExpectsFailed = 0;
int numAssertsFailed = 0;
int numObjectsCaught = 0;

namespace {
bool test(bool isExpect, const char *suffix, bool condition,
          const char *conditionString, const char *file, int line,
          const char *testName) {
  if (isExpect) {
    ++numExpects;
  } else {
    ++numAsserts;
  }

  if (!condition) {
    std::cout << file << ":" << line << ": " << testName << ": ";

    if (isExpect) {
      ++numExpectsFailed;

      std::cout << "TLO_EXPECT";
    } else {
      ++numAssertsFailed;

      std::cout << "TLO_ASSERT";
    }

    std::cout << suffix << " failed: " << conditionString << std::endl;
  }

  return condition;
}
}  // namespace

bool expect(bool isExpect, bool condition, const char *conditionString,
            const char *file, int line, const char *testName) {
  return test(isExpect, "", condition, conditionString, file, line, testName);
}

bool expectFalse(bool isExpect, bool condition, const char *conditionString,
                 const char *file, int line, const char *testName) {
  return test(isExpect, "_FALSE", !condition, conditionString, file, line,
              testName);
}
}  // namespace internal

void runTests() {
  const std::deque<const Test *> &tests = internal::getTests();
  for (const Test *test : tests) {
    try {
      std::cout << "[ RUNNING ] " << test->testName() << std::endl;
      test->run();
      std::cout << "[ DONE    ] " << test->testName() << std::endl;
    } catch (const std::exception &e) {
      ++internal::numObjectsCaught;
      std::cout << e.what() << std::endl;
      std::cout << "[ ERROR   ] " << test->testName() << " threw an exception."
                << std::endl;
    } catch (...) {
      ++internal::numObjectsCaught;
      std::cout << "[ ERROR   ] " << test->testName() << " threw an object."
                << std::endl;
    }
  }
}

void printReport() {
  std::cout << "=========================" << std::endl;
  std::cout << "# Expects       : " << internal::numExpects << std::endl;
  std::cout << "# Asserts       : " << internal::numAsserts << std::endl;
  std::cout << "# Expects failed: " << internal::numExpectsFailed << std::endl;
  std::cout << "# Asserts failed: " << internal::numAssertsFailed << std::endl;
  std::cout << "# Objects caught: " << internal::numObjectsCaught << std::endl;
  std::cout << "=========================" << std::endl;
}

void exit() {
  if (internal::numExpectsFailed || internal::numAssertsFailed ||
      internal::numObjectsCaught) {
    std::cout << "[ FAILED  ]" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::cout << "[ PASSED  ]" << std::endl;
  std::exit(EXIT_SUCCESS);
}
}  // namespace test
}  // namespace tlo
