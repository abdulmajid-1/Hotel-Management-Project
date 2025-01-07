#include <iostream>
#include <vector>
#include "user.hpp"
using namespace std;

struct RoomNode
{
  // all the information about room goes here
  // this will be implemented as a linked list
  // currently we have only one information that is room no.
  int room_no;
  int user_id;
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
    UserActions::require_admin();

    root->children.push_back(new FloorNode(++floors, current_user->id));
  }
};
