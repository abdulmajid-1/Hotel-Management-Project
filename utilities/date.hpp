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

  cout << timeSinceEpoch << endl;

  // Convert to milliseconds
  long millisecondsSinceEpoch = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::from_time_t(timeSinceEpoch).time_since_epoch()).count();

  return millisecondsSinceEpoch;
}
