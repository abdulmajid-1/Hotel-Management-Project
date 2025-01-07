#include <iostream>
#include <vector>
using namespace std;

enum UserRole
{
  ADMIN,
  USER
};

struct User
{
  int id; // id and user_name both should be unique
  string user_name;
  string password;
  UserRole role = USER;
  User(int id, string user_name, string password) : id(id), user_name(user_name), password(password) {}
};

User *current_user = NULL;

class UserActions
{
  static vector<User *> users;

public:
  static void sign_in(string user_name, string password)
  {
    for (User *user : users)
    {
      if (user->user_name == user_name && user->password == password)
      {
        cout << "You are signed IN." << endl;
        current_user = user;
        return;
      }
    }
    cout << "Username of password Incorrect." << endl;
  }

  static void signup(string user_name, string password)
  {
    // first find if this user with this user_name doesn't exists
    for (const User *user : users)
    {
      if (user->user_name == user_name)
      {
        cout << "User Already Exists." << endl;
        return;
      }
    }

    int id;
    if (users.size() == 0)
      id = 0;
    else
      id = users[users.size() - 1]->id + 1;
    User *new_user = new User(id, user_name, password);
  }

  static bool require_auth()
  {
    if (!current_user)
      throw "Not Authenticated";
  }

  static bool require_admin()
  {
    if (!current_user || current_user->role != ADMIN)
      throw "Only Admin has Access";
  }
};
