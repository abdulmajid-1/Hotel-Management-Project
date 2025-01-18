#pragma once

#include <iostream>
#include <vector>
#include "../utilities/utils.hpp"
#include "persistor.hpp"

using namespace std;

class Booking
{
  const string BOOKING_TABLE_NAME = "bookings";
  int floor_no;
  int room_no;
  int user_id;       // who booked the room
  string start_date; // (DD/MM/YYYY)
  string end_date;   // (DD/MM/YYYY)

public:
  Booking(int floor_no, int room_no, int user_id, string start_date, string end_date)
      : floor_no(floor_no), room_no(room_no), user_id(user_id), start_date(start_date), end_date(end_date) {}

  void save()
  {
    // FLOOR_NO, ROOM_NO, USER_ID, START_DATE, END_DATE
    vector<string> row = {to_string(floor_no), to_string(room_no), to_string(user_id), start_date, end_date};

    Persistor::save(BOOKING_TABLE_NAME, row);
  }
};
