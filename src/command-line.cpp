#include "tlo-cpp/command-line.hpp"

#include <cassert>
#include <exception>
#include <filesystem>
#include <stdexcept>
#include <utility>

#include "container.hpp"

namespace fs = std::filesystem;

namespace tlo {
std::ostream &operator<<(std::ostream &ostream, const OptionDetails &details) {
  return print(ostream << "{values: ", details.values)
         << ", lastIndex: " << details.lastIndex << '}';
}

bool operator==(const OptionDetails &details1, const OptionDetails &details2) {
  return details1.values == details2.values &&
         details1.lastIndex == details2.lastIndex;
}

CommandLine::CommandLine(
    int argc, char **argv,
    const std::map<std::string, OptionAttributes> &validOptions)
    : validOptions_(validOptions) {
  program_ = fs::path(argv[0]).stem().string();

  for (int i = 1; i < argc; ++i) {
    std::string argument = argv[i];

    if (!(argument.size() >= 3 && argument[0] == '-' && argument[1] == '-')) {
      arguments_.push_back(std::move(argument));
      continue;
    }

    auto equalPosition = argument.find("=");

    if (equalPosition == std::string::npos) {
      const auto iterator = validOptions_.find(argument);

      if (iterator == validOptions_.end()) {
        throw std::runtime_error("Error: \"" + argument +
                                 "\" is not a valid option.");
      }

      const auto &attributes = iterator->second;

      if (!attributes.valueRequired) {
        OptionDetails &details = options_[std::move(argument)];

        details.values.push_back("");
        details.lastIndex = i;
        continue;
      }

      if (i + 1 >= argc) {
        throw std::runtime_error("Error: Option \"" + argument +
                                 "\" requires a value.");
      }

      OptionDetails &details = options_[std::move(argument)];

      i++;
      details.values.push_back(argv[i]);
      details.lastIndex = i - 1;
    } else {
      std::string option = argument.substr(0, equalPosition);
      const auto iterator = validOptions_.find(option);

      if (iterator == validOptions_.end()) {
        throw std::runtime_error("Error: \"" + option +
                                 "\" is not a valid option.");
      }

      const auto &attributes = iterator->second;

      if (!attributes.valueRequired) {
        throw std::runtime_error("Error: Option \"" + option +
                                 "\" does not take values.");
      }

      auto nextPosition = equalPosition + 1;
      auto value =
          argument.substr(nextPosition, argument.size() - nextPosition);
      OptionDetails &details = options_[std::move(option)];

      details.values.push_back(std::move(value));
      details.lastIndex = i;
    }
  }
}

const std::string &CommandLine::program() const { return program_; }

const std::unordered_map<std::string, OptionDetails> &CommandLine::options()
    const {
  return options_;
}

const std::vector<std::string> &CommandLine::arguments() const {
  return arguments_;
}

const std::map<std::string, OptionAttributes> &CommandLine::validOptions()
    const {
  return validOptions_;
}

void CommandLine::printValidOptions(std::ostream &ostream) const {
  if (!validOptions_.empty()) {
    ostream << "Options:" << std::endl;

    bool first = true;

    for (const auto &option : validOptions_) {
      if (!first) {
        ostream << std::endl;
      }

      ostream << "  " << option.first;

      if (option.second.valueRequired) {
        ostream << "=value";
      }

      ostream << std::endl;
      ostream << "    " << option.second.description << std::endl;

      if (first) {
        first = false;
      }
    }
  }
}

bool CommandLine::specifiedOption(const std::string &option) const {
  return options_.find(option) != options_.end();
}

const std::string &CommandLine::getOptionValue(
    const std::string &option) const {
  return options_.at(option).values.back();
}

constexpr int NUMBER_BASE = 10;

template <class Integer>
Integer getOptionsValueAsInteger(
    const CommandLine &commandLine, const std::string &option, Integer minValue,
    Integer maxValue,
    Integer (*stringToInteger)(const std::string &, std::size_t *, int)) {
  assert(minValue <= maxValue);

  const std::string &string = commandLine.getOptionValue(option);
  Integer value;

  try {
    value = stringToInteger(string, nullptr, NUMBER_BASE);
  } catch (const std::exception &) {
    throw std::runtime_error("Error: Cannot convert " + option + " value \"" +
                             string + "\" to integer.");
  }

  if (value < minValue) {
    throw std::runtime_error(
        "Error: " + option + " value " + std::to_string(value) +
        " is less than minimum value " + std::to_string(minValue) + ".");
  }

  if (value > maxValue) {
    throw std::runtime_error(
        "Error: " + option + " value " + std::to_string(value) +
        " is greater than maximum value " + std::to_string(maxValue) + ".");
  }

  return value;
}

unsigned long CommandLine::getOptionValueAsULong(const std::string &option,
                                                 unsigned long minValue,
                                                 unsigned long maxValue) const {
  return getOptionsValueAsInteger(*this, option, minValue, maxValue,
                                  std::stoul);
}

int CommandLine::getOptionValueAsInt(const std::string &option, int minValue,
                                     int maxValue) const {
  return getOptionsValueAsInteger(*this, option, minValue, maxValue, std::stoi);
}

const std::vector<std::string> &CommandLine::getOptionValues(
    const std::string &option) const {
  return options_.at(option).values;
}

int CommandLine::getOptionLastIndex(const std::string &option) const {
  return options_.at(option).lastIndex;
}

std::ostream &operator<<(std::ostream &ostream,
                         const CommandLine &commandLine) {
  ostream << "{program: " << commandLine.program() << ", ";
  print(ostream << "options: ", commandLine.options()) << ", ";
  print(ostream << "arguments: ", commandLine.arguments()) << '}';
  return ostream;
}
}  // namespace tlo
