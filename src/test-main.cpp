#include "tlo-cpp/test.hpp"

int main() {
  tlo::test::runTests();
  tlo::test::printReport();
  tlo::test::exit();
}
