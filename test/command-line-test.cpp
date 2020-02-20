#include <tlo-cpp/command-line.hpp>
#include <tlo-cpp/test.hpp>

namespace {
TLO_TEST(NothingGiven) {
  int argc = 1;
  const char *argv[] = {"program"};
  const tlo::CommandLine commandLine(argc, argv);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT_EQ(commandLine.options().size(), 0U);
  TLO_EXPECT_EQ(commandLine.arguments().size(), 0U);
}

using ArgumentsVector = std::vector<std::string>;

TLO_TEST(OnlyArgumentsGiven) {
  int argc = 3;
  const char *argv[] = {"program", "arg1", "arg2"};
  const tlo::CommandLine commandLine(argc, argv);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT_EQ(commandLine.options().size(), 0U);
  TLO_EXPECT(commandLine.arguments() == ArgumentsVector({"arg1", "arg2"}));
}

TLO_TEST(OptionValueNotRequiredButGiven) {
  int argc = 3;
  const char *argv[] = {"program", "--op1=value1", "--op2=value2"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {false, ""}}};

  try {
    const tlo::CommandLine commandLine(argc, argv, validOptions);
    TLO_EXPECT(false);
  } catch (...) {
  }
}

using OptionsMap = std::unordered_map<std::string, tlo::OptionDetails>;

TLO_TEST(OptionValueNotRequiredAndNotGiven) {
  int argc = 3;
  const char *argv[] = {"program", "--op1=value1", "--op2"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {false, ""}}};
  const tlo::CommandLine commandLine(argc, argv, validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() ==
             OptionsMap({{"--op1", {{"value1"}, 1}}, {"--op2", {{""}, 2}}}));
  TLO_EXPECT_EQ(commandLine.arguments().size(), 0U);
}

TLO_TEST(OptionValueRequiredAndGiven) {
  int argc = 3;
  const char *argv[] = {"program", "--op1=value1", "--op2=value2"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};
  const tlo::CommandLine commandLine(argc, argv, validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() == OptionsMap({{"--op1", {{"value1"}, 1}},
                                                  {"--op2", {{"value2"}, 2}}}));
  TLO_EXPECT_EQ(commandLine.arguments().size(), 0U);
}

TLO_TEST(OptionValueRequiredAndGivenInNextArgument) {
  int argc = 5;
  const char *argv[] = {"program", "--op1", "value1", "--op2", "value2"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};
  const tlo::CommandLine commandLine(argc, argv, validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() == OptionsMap({{"--op1", {{"value1"}, 1}},
                                                  {"--op2", {{"value2"}, 3}}}));
  TLO_EXPECT_EQ(commandLine.arguments().size(), 0U);
}

TLO_TEST(OptionValueRequiredButNotGiven) {
  int argc = 4;
  const char *argv[] = {"program", "--op1", "value1", "--op2"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};

  try {
    const tlo::CommandLine commandLine(argc, argv, validOptions);
    TLO_EXPECT(false);
  } catch (...) {
  }
}

TLO_TEST(OptionInvalid) {
  int argc = 4;
  const char *argv[] = {"program", "--op1", "value1", "--op3"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};

  try {
    const tlo::CommandLine commandLine(argc, argv, validOptions);
    TLO_EXPECT(false);
  } catch (...) {
  }
}

TLO_TEST(OptionInvalidAndValueGiven) {
  int argc = 4;
  const char *argv[] = {"program", "--op1", "value1", "--op3=value3"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};

  try {
    const tlo::CommandLine commandLine(argc, argv, validOptions);
    TLO_EXPECT(false);
  } catch (...) {
  }
}

TLO_TEST(OptionsAndArgumentsGiven) {
  int argc = 5;
  const char *argv[] = {"program", "--op1", "arg1", "--op2=value2", "arg2"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {false, ""}}, {"--op2", {true, ""}}};
  const tlo::CommandLine commandLine(argc, argv, validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() ==
             OptionsMap({{"--op1", {{""}, 1}}, {"--op2", {{"value2"}, 3}}}));
  TLO_EXPECT(commandLine.arguments() == ArgumentsVector({"arg1", "arg2"}));
}

TLO_TEST(getOptionsValueAsInt) {
  int argc = 4;
  const char *argv[] = {"program", "--op1=", "--op2=2", "arg1"};
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};
  const tlo::CommandLine commandLine(argc, argv, validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() ==
             OptionsMap({{"--op1", {{""}, 1}}, {"--op2", {{"2"}, 2}}}));
  TLO_EXPECT(commandLine.arguments() == ArgumentsVector({"arg1"}));

  try {
    commandLine.getOptionValueAsInt("--op1", 0, 0);
    TLO_EXPECT(false);
  } catch (...) {
  }

  try {
    commandLine.getOptionValueAsInt("--op2", 3, 3);
    TLO_EXPECT(false);
  } catch (...) {
  }

  try {
    commandLine.getOptionValueAsInt("--op2", 1, 1);
    TLO_EXPECT(false);
  } catch (...) {
  }

  TLO_EXPECT_EQ(commandLine.getOptionValueAsInt("--op2", 2, 2), 2);
}
}  // namespace
