#pragma once

#include <chrono>
#include <sstream>
#include <iomanip>

using namespace std::chrono;

class Date
{
public:
  static long getCurrentTimestamp();
  static long convertToTimestamp(const string &dateStr);
  static long getMillisecondsSinceMidnight();
};

long Date::getCurrentTimestamp()
{
  auto now = system_clock::now();
  auto millisecondsSinceEpoch = duration_cast<milliseconds>(now.time_since_epoch()).count();

  return millisecondsSinceEpoch;
}

long Date::convertToTimestamp(const string &dateStr)
{
  // Parse the input string (dd/mm/yyyy)
  tm tm = {};
  istringstream ss(dateStr);
  ss >> get_time(&tm, "%d/%m/%Y");

  if (ss.fail())
  {
    throw invalid_argument("Invalid date format. Use dd/mm/yyyy.");
  }

  // Convert tm to time_t (seconds since epoch)
  time_t timeSinceEpoch = mktime(&tm);
  if (timeSinceEpoch == -1)
  {
    throw runtime_error("Failed to convert time.");
  }

  // Convert to milliseconds
  long millisecondsSinceEpoch = duration_cast<milliseconds>(system_clock::from_time_t(timeSinceEpoch).time_since_epoch()).count();

  return millisecondsSinceEpoch;
}

long Date::getMillisecondsSinceMidnight()
{
  // Get current time
  auto now = system_clock::now();
  time_t nowTimeT = system_clock::to_time_t(now);

  // Extract today's date and reset time to 12:00 AM
  tm tm = *localtime(&nowTimeT);
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  tm.tm_isdst = -1;

  time_t midnightTimeT = mktime(&tm);

  if (midnightTimeT == -1)
  {
    throw runtime_error("Failed to calculate midnight time.");
  }

  // Calculate milliseconds since midnight
  auto midnight = system_clock::from_time_t(midnightTimeT);
  long millisecondsSinceMidnight = duration_cast<milliseconds>(now - midnight).count();

  return millisecondsSinceMidnight;
}
