#include <iostream>
#include "user.hpp"
#include "hotel.hpp"
using namespace std;

void show_admin_options()
{
  vector<string> options = {"Create Floor", "Add room", "Show All Floors", "Signout"};
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

int main()
{
  // get the persisted data from the disk
  UserActions::populate_users();

  Hotel hotel;
  manage_auth();

  // reaching here means, we have successfully signed in.
  // now the user can be admin or user
  // first handle the admin user
  // Reaching here means we have successfully signed in
  if (UserActions::current_user && UserActions::current_user->role == ADMIN)
  {
    char c = '\0';
    while (tolower(c) != 'q')
    {
      try
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
          hotel.add_room(n);
          cout << "Room is added to floor #" << n << endl;
          break;

        case 'c':
          hotel.show_all_floors();
          break;
        case 'd':
          UserActions::signout();
          manage_auth();
          break;

        case 'q':
          exit(0);

        default:
          cout << "Invalid option. Try again." << endl;
          break;
        }
      }

      catch (const char *&msg)
      {
        cerr << "Error: " << msg << endl;
      }
      catch (const exception &e)
      {
        cerr << "Exception: " << e.what() << endl;
        exit(1);
      }
    }

    return 0;
  }
}
