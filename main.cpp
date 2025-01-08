#include <iostream>
#include "user.hpp"
using namespace std;

int main()
{
  try
  {
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

    // after reaching here means, we have successfully signed in.
  }
  catch (const exception &e)
  {
    cerr << "Error: " << e.what() << endl;
  }

  return 0;
}
