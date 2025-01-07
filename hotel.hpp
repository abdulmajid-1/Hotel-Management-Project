#include <iostream>
#include <vector>
using namespace std;

// Tree root is the hotel manager room, that manages all the tree

struct RoomNode
{
  // all the information about room goes here
  // this will be implemented as a linked list
  // currently we have only one information that is room no.
  string room_no = "";
  RoomNode(string groom_no) : room_no(room_no) {}
};

struct FloorNode
{
  string room_no = ""; // if empty is it root node, root node doesn't have any function,

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
  FloorNode *root;
};
