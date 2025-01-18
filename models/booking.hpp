#pragma once

#include <iostream>
#include <vector>
#include "../utilities/utils.hpp"
#include "persistor.hpp"

using namespace std;

class Booking
{
  const string BOOKING_TABLE_NAME = "bookings";

public:
  int floor_no;
  int room_no;
  int user_id;       // who booked the room
  string start_date; // (DD/MM/YYYY)
  string end_date;   // (DD/MM/YYYY)

  Booking(int floor_no, int room_no, int user_id, string start_date, string end_date)
      : floor_no(floor_no), room_no(room_no), user_id(user_id), start_date(start_date), end_date(end_date) {}

  void save()
  {
    // FLOOR_NO, ROOM_NO, USER_ID, START_DATE, END_DATE
    vector<string> row = {to_string(floor_no), to_string(room_no), to_string(user_id), start_date, end_date};

    Persistor::save(BOOKING_TABLE_NAME, row);
  }

  static const string BOOKING_TABLE_NAME_STATIC;
  static vector<Booking> get_all_bookings();
  static vector<Booking> get_booking_by_floor_and_room_no(int floor_no, int room_no);
  static vector<Booking> get_booking_by_user_id(int user_id);
};

const string Booking::BOOKING_TABLE_NAME_STATIC = "bookings";

vector<Booking> Booking::get_all_bookings()
{
  vector<Booking> bookings;

  vector<vector<string>> rows = Persistor::get_table(BOOKING_TABLE_NAME_STATIC);
  for (const vector<string> &row : rows)
  {
    Booking booking(stoi(row[0]), stoi(row[1]), stoi(row[2]), row[3], row[4]);
    bookings.push_back(booking);
  }

  return bookings;
}

vector<Booking> Booking::get_booking_by_floor_and_room_no(int floor_no, int room_no)
{
  vector<Booking> results;

  for (Booking booking : get_all_bookings())
  {
    if (booking.floor_no == floor_no && booking.room_no == room_no)
      results.push_back(booking);
  }

  return results;
}

vector<Booking> Booking::get_booking_by_user_id(int user_id)
{
  vector<Booking> results;

  for (Booking booking : get_all_bookings())
  {
    if (booking.user_id == user_id)
      results.push_back(booking);
  }

  return results;
}
