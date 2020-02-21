#include <initializer_list>
#include <tlo-cpp/command-line.hpp>
#include <tlo-cpp/test.hpp>

namespace {
std::vector<char *> makeArgv(std::initializer_list<const char *> strings) {
  std::vector<char *> arguments;

  for (const char *string : strings) {
    arguments.push_back(const_cast<char *>(string));
  }

  return arguments;
}
}  // namespace

namespace {
TLO_TEST(NothingGiven) {
  auto argv = makeArgv({"program", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const tlo::CommandLine commandLine(argc, argv.data());

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT_EQ(commandLine.options().size(), 0U);
  TLO_EXPECT_EQ(commandLine.arguments().size(), 0U);
}

using ArgumentsVector = std::vector<std::string>;

TLO_TEST(OnlyArgumentsGiven) {
  auto argv = makeArgv({"program", "arg1", "arg2", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const tlo::CommandLine commandLine(argc, argv.data());

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT_EQ(commandLine.options().size(), 0U);
  TLO_EXPECT(commandLine.arguments() == ArgumentsVector({"arg1", "arg2"}));
}

TLO_TEST(OptionValueNotRequiredButGiven) {
  auto argv = makeArgv({"program", "--op1=value1", "--op2=value2", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {false, ""}}};

  try {
    const tlo::CommandLine commandLine(argc, argv.data(), validOptions);
    TLO_EXPECT(false);
  } catch (...) {
  }
}

using OptionsMap = std::unordered_map<std::string, tlo::OptionDetails>;

TLO_TEST(OptionValueNotRequiredAndNotGiven) {
  auto argv = makeArgv({"program", "--op1=value1", "--op2", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {false, ""}}};
  const tlo::CommandLine commandLine(argc, argv.data(), validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() ==
             OptionsMap({{"--op1", {{"value1"}, 1}}, {"--op2", {{""}, 2}}}));
  TLO_EXPECT_EQ(commandLine.arguments().size(), 0U);
}

TLO_TEST(OptionValueRequiredAndGiven) {
  auto argv = makeArgv({"program", "--op1=value1", "--op2=value2", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};
  const tlo::CommandLine commandLine(argc, argv.data(), validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() == OptionsMap({{"--op1", {{"value1"}, 1}},
                                                  {"--op2", {{"value2"}, 2}}}));
  TLO_EXPECT_EQ(commandLine.arguments().size(), 0U);
}

TLO_TEST(OptionValueRequiredAndGivenInNextArgument) {
  auto argv =
      makeArgv({"program", "--op1", "value1", "--op2", "value2", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};
  const tlo::CommandLine commandLine(argc, argv.data(), validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() == OptionsMap({{"--op1", {{"value1"}, 1}},
                                                  {"--op2", {{"value2"}, 3}}}));
  TLO_EXPECT_EQ(commandLine.arguments().size(), 0U);
}

TLO_TEST(OptionValueRequiredButNotGiven) {
  auto argv = makeArgv({"program", "--op1", "value1", "--op2", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};

  try {
    const tlo::CommandLine commandLine(argc, argv.data(), validOptions);
    TLO_EXPECT(false);
  } catch (...) {
  }
}

TLO_TEST(OptionInvalid) {
  auto argv = makeArgv({"program", "--op1", "value1", "--op3", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};

  try {
    const tlo::CommandLine commandLine(argc, argv.data(), validOptions);
    TLO_EXPECT(false);
  } catch (...) {
  }
}

TLO_TEST(OptionInvalidAndValueGiven) {
  auto argv = makeArgv({"program", "--op1", "value1", "--op3=value3", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};

  try {
    const tlo::CommandLine commandLine(argc, argv.data(), validOptions);
    TLO_EXPECT(false);
  } catch (...) {
  }
}

TLO_TEST(OptionsAndArgumentsGiven) {
  auto argv =
      makeArgv({"program", "--op1", "arg1", "--op2=value2", "arg2", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {false, ""}}, {"--op2", {true, ""}}};
  const tlo::CommandLine commandLine(argc, argv.data(), validOptions);

  TLO_EXPECT_EQ(commandLine.program(), "program");
  TLO_EXPECT(commandLine.options() ==
             OptionsMap({{"--op1", {{""}, 1}}, {"--op2", {{"value2"}, 3}}}));
  TLO_EXPECT(commandLine.arguments() == ArgumentsVector({"arg1", "arg2"}));
}

TLO_TEST(getOptionsValueAsInt) {
  auto argv = makeArgv({"program", "--op1=", "--op2=2", "arg1", nullptr});
  int argc = static_cast<int>(argv.size()) - 1;
  const std::map<std::string, tlo::OptionAttributes> validOptions = {
      {"--op1", {true, ""}}, {"--op2", {true, ""}}};
  const tlo::CommandLine commandLine(argc, argv.data(), validOptions);

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
