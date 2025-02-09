#include <iostream>
#include "models/user.hpp"
#include "models/hotel.hpp"
using namespace std;

void show_admin_options()
{
  vector<string> options = {"Create Floor", "Add room", "Show All Floors", "Show All Rooms", "Update Room", "Show All Users", "Show All Bookings", "Delete Booking", "Delete User", "Signout", "Clear"};
  for (int i = 0; i < options.size(); i++)
    cout << char(i + 'a') << ": " << options[i] << endl;
}

void show_user_options()
{
  vector<string> options = {"Show All Rooms", "Show Rooms in Dates", "Book Room", "Show my Bookings", "Signout", "Clear"};
  for (int i = 0; i < options.size(); i++)
    cout << char(i + 'a') << ": " << options[i] << endl;
}

void manage_auth()
{
  // Sign in or Sign Up till current_user is NULL
  while (!UserActions::current_user)
  {
    int ans;
    cout << "Sign in or Signup." << endl;
    cout << "Press 1: for Signin, 2 for Signup: ";
    cin >> ans;

    cin.ignore();

    if (ans < 1 || ans > 2)
      continue;

    string user_name;
    string password;
    cout << "Enter user_name: ";
    getline(cin, user_name);
    cout << "Enter password: ";
    getline(cin, password);

    if (ans == 1)
      UserActions::sign_in(user_name, password);
    else
      UserActions::signup(user_name, password);
  }
}

void handle_admin_functionality(Hotel &hotel)
{
  char c = '\0';
  while (tolower(c) != 'q')
  {
    cout << "Press S to show options and Q to quit: ";
    cin >> c;
    c = tolower(c);

    switch (c)
    {
    case 's':
      show_admin_options();
      break;

    case 'a':
      hotel.add_floor();
      cout << "New Floor has been added." << endl;
      break;

    case 'b':
      int n;
      cout << "Enter Floor no.: ";
      cin >> n;
      cout << "Enter room type." << endl;
      cout << "'S' for SINGLE, 'D' for DOUBLE, 'U' for SUITE: ";
      char c;
      cin >> c;
      c = tolower(c);
      RoomType type;
      if (c == 's')
        type = SINGLE;
      else if (c == 'd')
        type = DOUBLE;
      else if (c == 'u')
        type = SUITE;
      hotel.add_room(n, type);
      cout << "Room is added to floor #" << n << endl;
      break;

    case 'c':
      hotel.show_all_floors();
      break;

    case 'd':
      hotel.show_all_rooms_admin();
      break;

    case 'e':
      cout << "Enter floor no: ";
      int floor_no;
      int room_no;
      cin >> floor_no;
      cout << "Enter room no: ";
      cin >> room_no;
      cout << "Enter new Room Type: " << endl;
      cout << "'S' for SINGLE, 'D' for DOUBLE, 'U' for SUITE: ";
      char s;
      cin >> s;
      s = tolower(s);
      RoomType roomType;
      if (s == 's')
        roomType = SINGLE;
      else if (s == 'd')
        roomType = DOUBLE;
      else
        roomType = SUITE;

      hotel.update_room(floor_no, room_no, roomType);
      cout << "Floor #" << floor_no << ", Room_no #" << room_no << " updated" << endl;

      break;

    case 'f':
      hotel.show_all_users();
      break;

    case 'g':
      hotel.show_all_bookings();
      break;

    case 'h':
      cout << "Enter booking id: ";
      int booking_id;
      cin >> booking_id;
      hotel.delete_booking(booking_id);
      break;

    case 'i':
      // DELETE THE USER, AND ITS BOOKINGS.
      cout << "Enter user id: ";
      int user_id;
      cin >> user_id;
      hotel.delete_user(user_id);
      break;

    case 'j':
      UserActions::signout();
      return;

    case 'k':
      clear();
      break;

    case 'q':
      exit(0);

    default:
      cout << "Invalid option. Try again." << endl;
      break;
    }
  }
}

void handle_user_functionality(Hotel &hotel)
{
  char c = '\0';
  while (tolower(c) != 'q')
  {
    cout << "Press S to show options and Q to quit: ";
    cin >> c;
    c = tolower(c);
    string start_date;
    string end_date;

    switch (c)
    {
    case 's':
      show_user_options();
      break;

    case 'a':
      hotel.show_all_rooms_user();
      break;

    case 'b':
      cout << "Enter staring Date (DD/MM/YYYY): ";
      cin >> start_date;
      cout << "Enter end Date (DD/MM/YYYY): ";
      cin >> end_date;
      hotel.show_rooms_available(start_date, end_date);
      break;

    case 'c':
      cout << "Enter floor #: ";
      int floor_no;
      cin >> floor_no;

      cout << "Enter room #: ";
      int room_no;
      cin >> room_no;

      cout << "Enter staring Date (DD/MM/YYYY): ";
      cin >> start_date;
      cout << "Enter end Date (DD/MM/YYYY): ";
      cin >> end_date;

      hotel.book_room(floor_no, room_no, start_date, end_date);
      break;

    case 'd':
      hotel.show_my_bookings();
      break;

    case 'e':
      UserActions::signout();
      return;

    case 'f':
      clear();
      break;

    case 'q':
      exit(0);

    default:
      cout << "Invalid option. Try again." << endl;
      break;
    }
  }
}

int main()
{
  // get the persisted data from the disk
  UserActions::populate_users();

  Hotel hotel;
  bool running = true;

  while (running)
  {
    // Manage authentication
    manage_auth();

    try
    {
      if (UserActions::current_user)
      {
        // Handle functionality based on the role
        if (UserActions::current_user->role == ADMIN)
          handle_admin_functionality(hotel);
        else
          handle_user_functionality(hotel);
      }
    }
    catch (const char *&msg)
    {
      cout << "Error: " << msg << endl;
    }
    catch (const exception &e)
    {
      cout << "Exception: " << e.what() << endl;
      continue;
    }

    // if signout is called, means current_user will be null, so keep running
    if (!UserActions::current_user)
      cout << "User signed out. Returning to main menu.\n";
    // if there is current_user, but we still reach here, means it is the end of
    // application, end it
    else
      running = false;
  }

  return 0;
}
