#pragma once

#include <iostream>
#include <vector>
#include "user.hpp"
using namespace std;

struct RoomNode
{
  // all the information about room goes here
  // this will be implemented as a linked list
  int room_no;
  // who created this room
  int user_id;
  RoomNode *next = NULL;

  RoomNode(int room_no, int user_id) : room_no(room_no), user_id(user_id) {}
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
};

// this will be the tree
class Hotel
{
  // this is the manager root, this will manage all the floors and rooms
  FloorNode *root = new FloorNode(0, 0);
  int floors = 0;
  const string FLOOR_TABLE_NAME = "floors";
  const string ROOM_TABLE_NAME = "rooms";

public:
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
      // ROOM NO, USER_ID, FLOOR_NO
      int which_floor = stoi(row[2]);

      if (which_floor > root->children.size() || which_floor < 1)
      {
        cout << "Invalid Room Data." << endl;
        exit(1);
      }

      int counter = 0;
      FloorNode *current_floor = root->children[counter];
      while (counter < root->children.size() && counter < which_floor - 1)
        current_floor = root->children[++counter];

      RoomNode *new_room = new RoomNode(stoi(row[0]), stoi(row[1]));
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

  void add_room(int which_floor)
  {
    if (which_floor > root->children.size() || which_floor < 1)
    {
      cout << "Floor doesn't exist." << endl;
      return;
    }

    int counter = 0;
    FloorNode *current_floor = root->children[counter];
    while (counter < root->children.size() && counter < which_floor - 1)
      current_floor = root->children[++counter];

    vector<string> room_values;

    RoomNode *curr_room = current_floor->rooms;
    if (!curr_room)
    {
      RoomNode *new_room = new RoomNode(1, UserActions::current_user->id);
      current_floor->rooms = new_room;

      room_values.push_back(to_string(new_room->room_no));
      room_values.push_back(to_string(new_room->user_id));
      room_values.push_back(to_string(which_floor));
    }
    else
    {
      while (curr_room->next != NULL)
        curr_room = curr_room->next;
      RoomNode *new_room = new RoomNode(curr_room->room_no + 1, UserActions::current_user->id);
      curr_room->next = new_room;

      room_values.push_back(to_string(new_room->room_no));
      room_values.push_back(to_string(new_room->user_id));
      room_values.push_back(to_string(which_floor));
    }

    // room values are
    // ROOM NO, USER_ID, FLOOR_NO
    Persistor::save(ROOM_TABLE_NAME, room_values);
  }
};
