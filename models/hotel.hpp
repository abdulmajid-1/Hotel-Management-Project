#pragma once

#include <iostream>
#include <vector>
#include "user.hpp"
#include "booking.hpp"
#include "../utilities/date.hpp"
using namespace std;

enum RoomType
{
  SINGLE,
  DOUBLE,
  SUITE,
};

string typeToString(RoomType type)
{
  switch (type)
  {
  case SINGLE:
    return "SINGLE";
  case DOUBLE:
    return "DOUBLE";
  case SUITE:
    return "SUITE";
  default:
    return "UNKNOWN";
  }
}

RoomType stringToType(std::string str)
{
  if (str == "SINGLE")
    return SINGLE;
  else if (str == "DOUBLE")
    return DOUBLE;
  else if (str == "SUITE")
    return SUITE;

  throw "Unknown room type string";
}

struct RoomNode
{
  // all the information about room goes here
  // this will be implemented as a linked list
  int room_no;
  // who created this room
  int user_id;
  RoomType type;

  RoomNode *next = NULL;

  RoomNode(int room_no, int user_id, RoomType type = SINGLE) : room_no(room_no), user_id(user_id), type(type) {}
};

struct FloorNode
{
  int floor_no; // if 0 is it root node, root node doesn't have any function
  int user_id;
  FloorNode(int floor_no, int user_id) : floor_no(floor_no), user_id(user_id) {}

  // 1st degree children will be floor
  vector<FloorNode *> children;

  // except root node, all the other nodes are floors so they should have linked list of rooms
  // we will add rooms later
  RoomNode *rooms;

  // whenever floor node is deleted, all the rooms
  ~FloorNode()
  {
    RoomNode *temp = rooms;
    while (temp)
    {
      RoomNode *next = temp->next;
      delete temp;
      temp = next;
    }
  }
};

// this will be the tree
class Hotel
{
  // this is the manager root, this will manage all the floors and rooms
  FloorNode *root = new FloorNode(0, 0);
  int floors = 0;
  const string FLOOR_TABLE_NAME = "floors";
  const string ROOM_TABLE_NAME = "rooms";

  void delete_hotel_nodes(FloorNode *root)
  {
    for (int i = 0; i < root->children.size(); i++)
      delete_hotel_nodes(root->children[i]);
    delete root;
  }

public:
  ~Hotel() { delete_hotel_nodes(root); }

  Hotel()
  {
    // getting all the floors from the disk, and adding them to the room
    vector<vector<string>> floors_raw_data = Persistor::get_table(FLOOR_TABLE_NAME);
    for (vector<string> row : floors_raw_data)
    {
      // values are
      // FLOOR NO, USER_ID
      floors++;
      root->children.push_back(new FloorNode(stoi(row[0]), stoi(row[1])));
    }

    // now get rooms, and store them in the respective floor
    vector<vector<string>> rooms_raw_data = Persistor::get_table(ROOM_TABLE_NAME);
    for (vector<string> row : rooms_raw_data)
    {
      // room values are
      // ROOM NO, USER_ID, TYPE, FLOOR_NO
      int which_floor = stoi(row[3]);

      if (which_floor > root->children.size() || which_floor < 1)
      {
        cout << "Invalid Room Data." << endl;
        exit(1);
      }

      int counter = 0;
      FloorNode *current_floor = root->children[counter];
      while (counter < root->children.size() && counter < which_floor - 1)
        current_floor = root->children[++counter];

      RoomNode *new_room = new RoomNode(stoi(row[0]), stoi(row[1]), stringToType(row[2]));
      if (!current_floor->rooms)
        current_floor->rooms = new_room;
      else
      {
        // go to the end of rooms and add it
        RoomNode *curr_room = current_floor->rooms;

        while (curr_room->next != NULL)
          curr_room->next;
        curr_room->next = new_room;
      }
    }
  }

  void add_floor()
  {
    // if we are checking for admin, we don't need to check for require_auth
    // because require_admin will explicitly check for require_auth
    UserActions::require_admin();
    FloorNode *new_floor = new FloorNode(++floors, UserActions::current_user->id);
    root->children.push_back(new_floor);

    vector<string> floor_values = {to_string(new_floor->floor_no), to_string(new_floor->user_id)};

    // saving the newly created floor to the disk
    // VALUES ARE:
    // FLOOR NO, USER_ID
    Persistor::save(FLOOR_TABLE_NAME, floor_values);
  }

  void show_all_floors()
  {
    cout << endl;
    vector<string> suffix = {"st", "nd", "rd"};
    for (int i = 0; i < root->children.size(); i++)
    {
      RoomNode *temp = root->children[i]->rooms;
      int room_count = 0;
      while (temp)
      {
        temp = temp->next;
        room_count++;
      }

      cout << i + 1 << (i <= 2 ? suffix[i] : "th") << " floor: " << room_count << " rooms" << endl;
    }
    cout << endl;
  }

  void show_all_rooms_admin()
  {
    UserActions::require_admin();
    vector<vector<string>> users = Persistor::get_table("users");

    cout << endl;
    for (int i = 0; i < root->children.size(); i++)
    {
      FloorNode *floor = root->children[i];
      vector<vector<string>> room_matrix;
      room_matrix.push_back(vector<string>{"Room No", "Type", "Created By"});
      RoomNode *temp_room = floor->rooms;

      while (temp_room)
      {
        // find the name of the user, who created the room
        string name = "";
        for (const vector<string> &row : users)
        {
          // row[0] is id, row[1] is the username of the user
          if (row[0] == to_string(temp_room->user_id))
            name = row[1];
        }

        vector<string> room_values = {
            to_string(temp_room->room_no),
            typeToString(temp_room->type),
            name,
        };
        room_matrix.push_back(room_values);
        temp_room = temp_room->next;
      }
      if (room_matrix.size() > 1)
      {
        cout << "Floor #" << root->children[i]->floor_no << endl;
        show_as_table(room_matrix);
      }
    }

    cout << endl;
  }

  void show_all_rooms_user()
  {
    vector<vector<string>> users = Persistor::get_table("users");

    cout << endl;
    for (int i = 0; i < root->children.size(); i++)
    {
      FloorNode *floor = root->children[i];
      vector<vector<string>> room_matrix;
      room_matrix.push_back(vector<string>{"Room No", "Type"});
      RoomNode *temp_room = floor->rooms;

      while (temp_room)
      {
        vector<string> room_values = {
            to_string(temp_room->room_no),
            typeToString(temp_room->type),
        };
        room_matrix.push_back(room_values);
        temp_room = temp_room->next;
      }
      if (room_matrix.size() > 1)
      {
        cout << "Floor #" << root->children[i]->floor_no << endl;
        show_as_table(room_matrix);
      }
    }

    cout << endl;
  }

  void add_room(int which_floor, RoomType type = SINGLE)
  {
    UserActions::require_admin();

    FloorNode *current_floor = NULL;
    for (int i = 0; i < root->children.size(); i++)
    {
      if (root->children[i]->floor_no == which_floor)
        current_floor = root->children[i];
    }

    if (!current_floor)
    {
      cout << "Floor #" << which_floor << " doesn't exist" << endl;
    }

    vector<string> room_values;

    RoomNode *curr_room = current_floor->rooms;
    if (!curr_room)
    {
      RoomNode *new_room = new RoomNode(1, UserActions::current_user->id, type);
      current_floor->rooms = new_room;

      room_values.push_back(to_string(new_room->room_no));
      room_values.push_back(to_string(new_room->user_id));
      room_values.push_back(typeToString(new_room->type));
      room_values.push_back(to_string(which_floor));
    }
    else
    {
      while (curr_room->next != NULL)
        curr_room = curr_room->next;
      RoomNode *new_room = new RoomNode(curr_room->room_no + 1, UserActions::current_user->id, type);
      curr_room->next = new_room;

      room_values.push_back(to_string(new_room->room_no));
      room_values.push_back(to_string(new_room->user_id));
      room_values.push_back(typeToString(new_room->type));
      room_values.push_back(to_string(which_floor));
    }

    // room values are
    // ROOM NO, USER_ID, TYPE, FLOOR_NO
    Persistor::save(ROOM_TABLE_NAME, room_values);
  }

  void update_room(int floor_no, int room_no, RoomType new_room_type)
  {
    UserActions::require_admin();

    // find the room with this floor_no and room_no
    RoomNode *founded = NULL;
    FloorNode *curr_floor = NULL;

    for (int i = 0; i < root->children.size(); i++)
    {
      if (root->children[i]->floor_no == floor_no)
        curr_floor = root->children[i];
    }

    if (!curr_floor)
    {
      cout << "Floor #" << floor_no << " not found." << endl;
      return;
    }

    RoomNode *curr_room = curr_floor->rooms;
    while (curr_room)
    {
      if (curr_room->room_no == room_no)
      {
        founded = curr_room;
        break;
      }
      curr_room = curr_room->next;
    }

    if (!curr_room)
    {
      cout << "Room #" << room_no << " not found." << endl;
      return;
    }

    curr_room->type = new_room_type;

    vector<vector<string>> rooms_serialized;
    // do it for all the floors
    for (int i = 0; i < root->children.size(); i++)
    {
      RoomNode *curr_room = root->children[i]->rooms;
      while (curr_room)
      {
        vector<string> room_values;
        room_values.push_back(to_string(curr_room->room_no));
        room_values.push_back(to_string(curr_room->user_id));
        room_values.push_back(typeToString(curr_room->type));
        room_values.push_back(to_string(root->children[i]->floor_no));

        rooms_serialized.push_back(room_values);

        curr_room = curr_room->next;
      }
    }

    Persistor::save_all(ROOM_TABLE_NAME, rooms_serialized);
  }

  void book_room(int floor_no, int room_no, string start_date, string end_date)
  {
    UserActions::require_auth();

    // first find the floor
    FloorNode *found_floor = NULL;
    for (int i = 0; i < root->children.size(); i++)
    {
      if (root->children[i]->floor_no == floor_no)
        found_floor = root->children[i];
    }

    if (!found_floor)
    {
      cout << "Floor #" << floor_no << " not found." << endl;
      return;
    }

    // find the room
    RoomNode *found_room = found_floor->rooms;
    while (found_room && found_room->room_no != room_no)
      found_room = found_room->next;

    if (!found_room)
    {
      cout << "Room #" << room_no << " not found." << endl;
      return;
    }

    // now check if the starting date is >= today
    if (Date::convertToTimestamp(start_date) < Date::getCurrentTimestamp() - Date::getMillisecondsSinceMidnight())
    {
      cout << "Error: Please enter valid starting date that has not Passed." << endl;
      return;
    }
    // now check if ending date is >= to starting date
    if (Date::convertToTimestamp(end_date) < Date::convertToTimestamp(start_date))
    {
      cout << "Error: Please enter Ending Date that is greater than Starting Date" << endl;
      return;
    }

    // Now check if between these dates (starting and ending), the room is already
    // booked or not, if room is booked between these dates, don't allow booking
    vector<Booking> curr_room_bookings = Booking::get_booking_by_floor_and_room_no(floor_no, room_no);

    for (const Booking &curr_room_booking : curr_room_bookings)
    {
      if (
          Date::convertToTimestamp(start_date) <= Date::convertToTimestamp(curr_room_booking.end_date) &&
          Date::convertToTimestamp(end_date) >= Date::convertToTimestamp(curr_room_booking.start_date))
      {
        cout << "Sorry. Room is already booked between these dates." << endl;
        return;
      }
    }

    // if all the checks are passed, create the booking
    (new Booking(floor_no, room_no, UserActions::current_user->id, start_date, end_date))->save();

    cout << "Your Room is booked Successfully." << endl;
  }

  void show_all_bookings()
  {
    UserActions::require_admin();
    vector<Booking> all_bookings = Booking::get_all_bookings();

    vector<vector<string>> matrix;
    matrix.push_back(vector<string>{"Booking ID", "Floor No.", "Room No.", "User ID", "Start Date", "End Date"});

    for (const Booking &booking : all_bookings)
    {
      matrix.push_back(vector<string>{
          to_string(booking.booking_id),
          to_string(booking.floor_no),
          to_string(booking.room_no),
          to_string(booking.user_id),
          booking.start_date,
          booking.end_date,
      });
    }

    show_as_table(matrix);
  }

  void show_my_bookings()
  {
    UserActions::require_auth();
    vector<Booking> bookings = Booking::get_booking_by_user_id(UserActions::current_user->id);

    vector<vector<string>> matrix;
    matrix.push_back(vector<string>{"Booking ID", "Floor No.", "Room No.", "Start Date", "End Date"});

    for (const Booking &booking : bookings)
    {
      matrix.push_back(vector<string>{
          to_string(booking.booking_id),
          to_string(booking.floor_no),
          to_string(booking.room_no),
          booking.start_date,
          booking.end_date,
      });
    }

    show_as_table(matrix);
  }

  void show_all_users()
  {
    UserActions::require_admin();
    vector<User *> users = UserActions::get_all_users();

    vector<vector<string>> user_matrix;
    user_matrix.push_back(vector<string>{"ID", "Username", "Role"});

    for (const User *user : users)
    {
      user_matrix.push_back(vector<string>{to_string(user->id), user->user_name, roleToString(user->role)});
    }

    show_as_table(user_matrix);
  }

  void show_rooms_available(string start_date, string end_date)
  {
    for (int i = 0; i < root->children.size(); i++)
    {
      vector<RoomNode *> available_rooms;

      RoomNode *curr_room = root->children[i]->rooms;
      while (curr_room)
      {
        vector<Booking> bookings = Booking::get_booking_by_floor_and_room_no(root->children[i]->floor_no, curr_room->room_no);

        // it needs to be passed by all bookings.
        bool is_passed = true;

        for (const Booking &booking : bookings)
        {
          // if there is only one booking at that date, don't show the room
          if (
              Date::convertToTimestamp(start_date) <= Date::convertToTimestamp(booking.end_date) &&
              Date::convertToTimestamp(end_date) >= Date::convertToTimestamp(booking.start_date))
            is_passed = false;
        }

        if (is_passed)
          available_rooms.push_back(curr_room);

        curr_room = curr_room->next;
      }

      if (available_rooms.size() != 0)
      {
        cout << "Floor #" << root->children[i]->floor_no << endl;
        vector<vector<string>> matrix;
        matrix.push_back(vector<string>{"Room NO.", "Type"});
        for (const RoomNode *room : available_rooms)
          matrix.push_back(vector<string>{to_string(room->room_no), typeToString(room->type)});

        show_as_table(matrix);
      }
    }
  }

  void delete_booking(int booking_id)
  {
    UserActions::require_admin();

    vector<Booking> all_bookings = Booking::get_all_bookings();
    vector<Booking> new_bookings;

    bool found = false;

    for (Booking booking : all_bookings)
    {
      if (booking.booking_id != booking_id)
        new_bookings.push_back(booking);
      else
        found = true;
    }

    if (found)
    {
      Booking::save_all(new_bookings);
      cout << "Booking with id: " << booking_id << " deleted." << endl;
    }
    else
      cout << "Booking with id: " << booking_id << " not found." << endl;
  }

  void delete_user(int user_id)
  {
    UserActions::require_admin();
    if (UserActions::current_user->id == user_id)
    {
      cout << "You cannot delete Yourself." << endl;
      return;
    }

    vector<User *> users = UserActions::get_all_users();
    vector<User> new_users;

    bool found = false;

    for (User *user : users)
    {
      if (user->id != user_id)
        new_users.push_back(*user);
      else
      {
        if (user->role == ADMIN)
        {
          cout << "You cannot delete an ADMIN." << endl;
          return;
        }
        found = true;
      }
    }

    if (found)
    {
      UserActions::save_all(new_users);
      cout << "User with id: " << user_id << " deleted." << endl;
      UserActions::populate_users();
      // now delete all its bookings, we don't need to delete its rooms because
      // rooms are created by admin, and deleting admins are not allowed
      vector<Booking> user_bookings = Booking::get_booking_by_user_id(user_id);
      for (const Booking &booking : user_bookings)
        delete_booking(booking.booking_id);
    }
    else
      cout << "User with id: " << user_id << " not found." << endl;
  }
};
