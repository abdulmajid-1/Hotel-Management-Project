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
  int booked_by = -1;
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

public:
  void add_floor()
  {
    // if we are checking for admin, we don't need to check for require_auth
    // because require_admin will explicitly check for require_auth
    // TODO: uncomment this
    // UserActions::require_admin();

    root->children.push_back(new FloorNode(++floors, UserActions::current_user->id));
  }

  void add_room(int which_floor)
  {
    int counter = 1;
    FloorNode *current_floor = root->children[counter];
    while (counter < root->children.size())
      current_floor = root->children[++counter];

    if (counter < which_floor)
    {
      cout << "Floor doesn't exists." << endl;
      return;
    }

    RoomNode *curr_room = current_floor->rooms;
    if (!curr_room)
    {
      RoomNode *new_room = new RoomNode(1, UserActions::current_user->id);
      current_floor->rooms = new_room;
    }
    else
    {
      while (curr_room->next != NULL)
        curr_room = curr_room->next;
      RoomNode *new_room = new RoomNode(curr_room->room_no + 1, UserActions::current_user->id);
      curr_room->next = new_room;
    }
  }
};
