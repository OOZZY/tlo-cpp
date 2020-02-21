#include <tlo-cpp/chrono.hpp>
#include <tlo-cpp/test.hpp>

namespace {
TLO_TEST(TimeToLocalTimestampAndBack) {
  std::time_t time = 0;

  TLO_EXPECT_EQ(tlo::localTimestampToTime(tlo::timeToLocalTimestamp(time)),
                time);
}

TLO_TEST(TimeToUtcTimestampAndBack) {
  std::time_t time = 0;

  TLO_EXPECT_EQ(tlo::utcTimestampToTime(tlo::timeToUtcTimestamp(time)), time);
}

TLO_TEST(LocalTimestampToTimeAndBack) {
  std::time_t time = 0;
  std::string timestamp = tlo::timeToLocalTimestamp(time);

  TLO_EXPECT_EQ(tlo::timeToLocalTimestamp(tlo::localTimestampToTime(timestamp)),
                timestamp);
}

TLO_TEST(UtcTimestampToTimeAndBack) {
  std::string timestamp = "1970-01-01 00:00:00 000 4 0";

  TLO_EXPECT_EQ(tlo::timeToUtcTimestamp(tlo::utcTimestampToTime(timestamp)),
                timestamp);
}

TLO_TEST(equalLocalTimestamps) {
  std::time_t time = 0;

  TLO_EXPECT(tlo::equalLocalTimestamps(tlo::timeToLocalTimestamp(time),
                                       tlo::timeToLocalTimestamp(time), 0));
}

TLO_TEST(equalUtcTimestamps) {
  std::time_t time = 0;

  TLO_EXPECT(tlo::equalUtcTimestamps(tlo::timeToUtcTimestamp(time),
                                     tlo::timeToUtcTimestamp(time), 0));

  std::string timestamp1 = "1970-01-01 00:00:00 000 4 0";
  std::string timestamp2 = "1970-01-01 00:00:02 000 4 0";

  TLO_EXPECT_FALSE(tlo::equalUtcTimestamps(timestamp1, timestamp2, 0));
  TLO_EXPECT_FALSE(tlo::equalUtcTimestamps(timestamp1, timestamp2, 1));
  TLO_EXPECT(tlo::equalUtcTimestamps(timestamp1, timestamp2, 2));
}
}  // namespace
