#ifndef TLO_CPP_STRING_HPP
#define TLO_CPP_STRING_HPP

#include <sstream>
#include <string>
#include <vector>

namespace tlo {
template <typename Char>
std::vector<std::basic_string<Char>> split(
    const std::basic_string<Char> &string, Char delimiter) {
  std::vector<std::basic_string<Char>> strings;
  std::vector<std::size_t> delimiterPositions;

  for (std::size_t i = 0; i < string.size(); ++i) {
    if (string[i] == delimiter) {
      delimiterPositions.push_back(i);
    }
  }

  if (delimiterPositions.empty()) {
    strings.push_back(string);
  } else {
    std::size_t subStrStartPosition;
    std::size_t subStrLength;

    subStrStartPosition = 0;
    subStrLength = delimiterPositions[0] - subStrStartPosition;
    strings.push_back(string.substr(subStrStartPosition, subStrLength));

    for (std::size_t i = 1; i < delimiterPositions.size(); ++i) {
      subStrStartPosition = delimiterPositions[i - 1] + 1;
      subStrLength = delimiterPositions[i] - subStrStartPosition;
      strings.push_back(string.substr(subStrStartPosition, subStrLength));
    }

    subStrStartPosition = delimiterPositions[delimiterPositions.size() - 1] + 1;
    subStrLength = string.size() - subStrStartPosition;
    strings.push_back(string.substr(subStrStartPosition, subStrLength));
  }

  return strings;
}

template <typename Char>
std::vector<std::basic_string<Char>> split(const Char *string, Char delimiter) {
  return split(std::basic_string<Char>(string), delimiter);
}

template <class T, typename Char = char>
std::basic_string<Char> toString(const T &object) {
  std::basic_ostringstream<Char> oss;

  oss << object;
  return oss.str();
}

// Join numOccurrences occurrences of string separated by separator.
template <typename Char>
std::basic_string<Char> join(std::size_t numOccurrences,
                             const std::basic_string<Char> &string,
                             const std::basic_string<Char> &separator) {
  std::basic_string<Char> newString;

  for (std::size_t i = 0; i < numOccurrences; ++i) {
    if (i > 0) {
      newString += separator;
    }

    newString += string;
  }

  return newString;
}

template <typename Char>
std::basic_string<Char> join(std::size_t numOccurrences, const Char *string,
                             const std::basic_string<Char> &separator) {
  return join(numOccurrences, std::basic_string<Char>(string), separator);
}

template <typename Char>
std::basic_string<Char> join(std::size_t numOccurrences,
                             const std::basic_string<Char> &string,
                             const Char *separator) {
  return join(numOccurrences, string, std::basic_string<Char>(separator));
}

template <typename Char>
std::basic_string<Char> join(std::size_t numOccurrences, const Char *string,
                             const Char *separator) {
  return join(numOccurrences, std::basic_string<Char>(string),
              std::basic_string<Char>(separator));
}
}  // namespace tlo

#endif  // TLO_CPP_STRING_HPP
