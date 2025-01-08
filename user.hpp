#pragma once

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

class UserActions
{
  static vector<User *> users;

public:
  static User *current_user;
  static void sign_in(string user_name, string password);
  static void signup(string user_name, string password);
  static void signout();
  static void require_auth();
  static void require_admin();
};
