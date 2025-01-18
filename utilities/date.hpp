#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

class Date
{
public:
  static long getCurrentTimestamp();
  static long convertToTimestamp(const string &dateStr);
  static long getMillisecondsSinceMidnight();
};

long Date::getCurrentTimestamp()
{
  auto now = std::chrono::system_clock::now();
  auto millisecondsSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

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
  long millisecondsSinceEpoch = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::from_time_t(timeSinceEpoch).time_since_epoch()).count();

  return millisecondsSinceEpoch;
}

long Date::getMillisecondsSinceMidnight()
{
  // Get current time
  auto now = chrono::system_clock::now();
  time_t nowTimeT = chrono::system_clock::to_time_t(now);

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
  auto midnight = chrono::system_clock::from_time_t(midnightTimeT);
  auto millisecondsSinceMidnight = chrono::duration_cast<chrono::milliseconds>(now - midnight).count();

  return millisecondsSinceMidnight;
}
