#ifndef TLO_CPP_CHRONO_HPP
#define TLO_CPP_CHRONO_HPP

#include <cassert>
#include <chrono>
#include <ctime>
#include <limits>
#include <string>

namespace tlo {
namespace internal {
template <class Duration, class Rep = typename Duration::rep>
constexpr Duration maxDuration() noexcept {
  return Duration{std::numeric_limits<Rep>::max()};
}

template <class Duration>
constexpr Duration absDuration(const Duration d) noexcept {
  return Duration{(d.count() < 0) ? -d.count() : d.count()};
}
}  // namespace internal

// Converts from one type of std::chrono::time_point to another, even if they
// are on different clocks. The conversion may have some error. Based on code
// from web page "Convert between c++11 clocks" available at:
// https://stackoverflow.com/questions/35282308/convert-between-c11-clocks
// (Retrieved February 8, 2020)
template <typename DstTimePoint, typename SrcTimePoint,
          typename DstDuration = typename DstTimePoint::duration,
          typename SrcDuration = typename SrcTimePoint::duration,
          typename DstClock = typename DstTimePoint::clock,
          typename SrcClock = typename SrcTimePoint::clock>
DstTimePoint convertTimePoint(
    const SrcTimePoint timePoint,
    const SrcDuration tolerance =
        std::chrono::duration_cast<SrcDuration>(std::chrono::nanoseconds{100}),
    const int limit = 10) {
  assert(limit > 0);

  SrcTimePoint srcNow;
  DstTimePoint dstNow;
  auto epsilon = internal::maxDuration<SrcDuration>();

  for (auto i = 0; epsilon > tolerance && i < limit; ++i) {
    const auto srcBefore = SrcClock::now();
    const auto dstBetween = DstClock::now();
    const auto srcAfter = SrcClock::now();
    const auto srcDiff = srcAfter - srcBefore;
    const auto delta = internal::absDuration(srcDiff);

    if (delta < epsilon) {
      srcNow = srcBefore + srcDiff / 2;
      dstNow = dstBetween;
      epsilon = delta;
    }
  }

  return dstNow + std::chrono::duration_cast<DstDuration>(timePoint - srcNow);
}

// Converts timeObject to a timestamp that preserves all the fields of the
// std::tm object. The beginning of the timestamp is in "%Y-%m-%d %H:%M:%S"
// format. Returns the timestamp.
std::string tmToTimestamp(const std::tm &timeObject);

// Calls std::localtime(&time) then passes the resulting std::tm object to
// tmToTimestamp(). Returns the timestamp. Uses the same mutex as other
// functions in this module that require it. Not thread-safe when called
// concurrently with external functions that also call std::localTime(),
// std::gmtime(), or std::ctime().
std::string timeToLocalTimestamp(std::time_t time);

// Calls std::gmtime(&time) then passes the resulting std::tm object to
// tmToTimestamp(). Returns the timestamp. Uses the same mutex as other
// functions in this module that require it. Not thread-safe when called
// concurrently with external functions that also call std::localTime(),
// std::gmtime(), or std::ctime().
std::string timeToUtcTimestamp(std::time_t time);

// Fills the tm according to the given timestamp. Assumes timestamp is in the
// format returned by tmToTimestamp(). Throws std::runtime_error if timestamp
// fails parsing. This function does the reverse of tmToTimestamp().
void timestampToTm(std::tm &timeObject, const std::string &timestamp);

// Calls timestampToTm(..., localTimestamp) then passes the filled std::tm
// object to std::mktime() and returns the result. Assumes localTimestamp was
// generated by timeToLocalTimestamp(). This function does the reverse of
// timeToLocalTimestamp().
std::time_t localTimestampToTime(const std::string &localTimestamp);

// Calls timestampToTm(..., utcTimestamp) then tries to convert the filled
// std::tm object to a time_t. Assumes utcTimestamp was generated by
// timeToUtcTimestamp(). This function does the reverse of
// timeToUtcTimestamp(). Uses the same mutex as other functions in this module
// that require it. Not thread-safe when called concurrently with external
// functions that also call std::localTime(), std::gmtime(), or std::ctime().
std::time_t utcTimestampToTime(const std::string &utcTimestamp);

// Returns whether the given timestamps differ by at most maxSecondDifference
// seconds. Assumes the timestamps were generated by timeToLocalTimestamp().
// Throws std::runtime_error if any of the timestamps fail parsing.
bool equalLocalTimestamps(const std::string &localTimestamp1,
                          const std::string &localTimestamp2,
                          int maxSecondDifference);

// Returns whether the given timestamps differ by at most maxSecondDifference
// seconds. Assumes the timestamps were generated by timeToUtcTimestamp().
// Throws std::runtime_error if any of the timestamps fail parsing. Uses the
// same mutex as other functions in this module that require it. Not
// thread-safe when called concurrently with external functions that also call
// std::localTime(), std::gmtime(), or std::ctime().
bool equalUtcTimestamps(const std::string &utcTimestamp1,
                        const std::string &utcTimestamp2,
                        int maxSecondDifference);
}  // namespace tlo

#endif  // TLO_CPP_CHRONO_HPP
