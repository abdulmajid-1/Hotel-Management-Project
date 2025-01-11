#include "user.hpp"

vector<User *> UserActions::users;
User *UserActions::current_user = NULL;

void UserActions::sign_in(string user_name, string password)
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

string roleToString(UserRole role)
{
  switch (role)
  {
  case USER:
    return "USER";
  case ADMIN:
    return "ADMIN";
  default:
    return "UNKNOWN";
  }
}

void UserActions::signup(string user_name, string password)
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
  users.push_back(new_user);
  current_user = new_user;
  cout << "You are signed up successfully." << endl;

  // WE HAVE TO EXPLICTLY REMEMBER THAT order or rows I.E.
  // ID, USERNAME, PASSWORD, ROLE
  vector<string> new_user_values = {
      to_string(new_user->id), new_user->user_name, new_user->password, roleToString(new_user->role)};
  Persister::save("users", new_user_values);
}

void UserActions::signout() { current_user = NULL; }

void UserActions::require_auth()
{
  if (!current_user)
    throw "Not Authenticated";
}

void UserActions::require_admin()
{
  if (!current_user || current_user->role != ADMIN)
    throw "Only Admin has Access";
}
