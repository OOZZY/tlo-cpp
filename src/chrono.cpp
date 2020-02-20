#include "tlo-cpp/chrono.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <stdexcept>

namespace tlo {
namespace {
std::mutex timeMutex;
constexpr int BASE_YEAR = 1900;
}  // namespace

std::string tmToTimestamp(const std::tm &timeObject) {
  std::ostringstream oss;

  oss.fill('0');
  oss << std::setw(4) << timeObject.tm_year + BASE_YEAR;
  oss << std::setw(1) << '-';
  oss << std::setw(2) << timeObject.tm_mon + 1;
  oss << std::setw(1) << '-';
  oss << std::setw(2) << timeObject.tm_mday;
  oss << std::setw(1) << ' ';
  oss << std::setw(2) << timeObject.tm_hour;
  oss << std::setw(1) << ':';
  oss << std::setw(2) << timeObject.tm_min;
  oss << std::setw(1) << ':';
  oss << std::setw(2) << timeObject.tm_sec;
  oss << std::setw(1) << ' ';
  oss << std::setw(3) << timeObject.tm_yday;
  oss << std::setw(1) << ' ';
  oss << std::setw(1) << timeObject.tm_wday;
  oss << std::setw(1) << ' ';
  oss << std::setw(1) << timeObject.tm_isdst;
  return oss.str();
}

std::string timeToLocalTimestamp(std::time_t time) {
  std::tm localTimeObjectCopy;

  std::unique_lock<std::mutex> timeUniqueLock(timeMutex);
  std::tm *localTimeObject = std::localtime(&time);

  std::memcpy(&localTimeObjectCopy, localTimeObject, sizeof(*localTimeObject));
  timeUniqueLock.unlock();

  return tmToTimestamp(localTimeObjectCopy);
}

std::string timeToUtcTimestamp(std::time_t time) {
  std::tm utcTimeObjectCopy;

  std::unique_lock<std::mutex> timeUniqueLock(timeMutex);
  std::tm *utcTimeObject = std::gmtime(&time);

  std::memcpy(&utcTimeObjectCopy, utcTimeObject, sizeof(*utcTimeObject));
  timeUniqueLock.unlock();

  return tmToTimestamp(utcTimeObjectCopy);
}

void timestampToTm(std::tm &timeObject, const std::string &timestamp) {
  std::istringstream iss(timestamp);

  iss >> timeObject.tm_year;
  iss.ignore(1);
  iss >> timeObject.tm_mon;
  iss.ignore(1);
  iss >> timeObject.tm_mday;
  iss.ignore(1);
  iss >> timeObject.tm_hour;
  iss.ignore(1);
  iss >> timeObject.tm_min;
  iss.ignore(1);
  iss >> timeObject.tm_sec;
  iss.ignore(1);
  iss >> timeObject.tm_yday;
  iss.ignore(1);
  iss >> timeObject.tm_wday;
  iss.ignore(1);
  iss >> timeObject.tm_isdst;

  if (iss.fail()) {
    throw std::runtime_error("Error: Failed to parse timestamp \"" + timestamp +
                             "\".");
  }

  timeObject.tm_year -= BASE_YEAR;
  timeObject.tm_mon--;
}

std::time_t localTimestampToTime(const std::string &localTimestamp) {
  std::tm localTimeObject;

  timestampToTm(localTimeObject, localTimestamp);
  return std::mktime(&localTimeObject);
}

bool equalLocalTimestamps(const std::string &localTimestamp1,
                          const std::string &localTimestamp2,
                          int maxSecondDifference) {
  if (localTimestamp1 == localTimestamp2) {
    return true;
  }

  std::time_t time1 = localTimestampToTime(localTimestamp1);
  std::time_t time2 = localTimestampToTime(localTimestamp2);
  double secondsDifference = std::abs(std::difftime(time1, time2));

  return secondsDifference <= maxSecondDifference;
}
}  // namespace tlo
