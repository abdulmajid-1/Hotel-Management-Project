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
    cout << "Sign in or Signup. Press 1: for Signin, 2 for Signup: ";
    cin >> ans;

    if (ans < 1 || ans > 2)
      continue;

    string user_name;
    string password;
    cout << "Enter user_name: ";
    cin >> user_name;
    cout << "Enter password: ";
    cin >> password;

    if (ans == 1)
      UserActions::sign_in(user_name, password);
    else
      UserActions::signup(user_name, password);
  }
}

int main()
{
  Hotel hotel;
  manage_auth();

  // reaching here means, we have successfully signed in.
  // now the user can be admin or user
  // first handle the admin user
  // TODO: CHANGE THIS TO ADMIN
  // Reaching here means we have successfully signed in
  if (UserActions::current_user && UserActions::current_user->role == USER)
  {
    char c = '\0';
    while (tolower(c) != 'q')
    {
      try
      {
        cout << "Press S to show options and Q to quit: ";
        cin >> c;
        c = tolower(c);

        if (c == 's')
        {
          show_admin_options();
        }
        else if (c == 'a')
        {
          hotel.add_floor();
          cout << "New Floor has been added." << endl;
        }
        else if (c == 'b')
        {
          int n;
          cout << "Enter Floor no.: ";
          cin >> n;
          hotel.add_room(n);
        }
        else if (c == 'd')
        {
          UserActions::signout();
          manage_auth();
        }
        else if (c != 'q')
        {
          cout << "Invalid option. Try again." << endl;
        }
      }

      catch (const char *&msg)
      {
        cerr << "Error: " << msg << endl;
      }
      catch (const exception &e)
      {
        cerr << "Exception: " << e.what() << endl;
      }
    }

    return 0;
  }
}
