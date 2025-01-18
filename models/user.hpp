#pragma once

#include <iostream>
#include <vector>
#include "persistor.hpp"
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
  const static string USER_TABLE_NAME;
  static vector<User *> users;

public:
  static void populate_users();
  static User *current_user;
  static void sign_in(string user_name, string password);
  static void signup(string user_name, string password);
  static void signout();
  static void require_auth();
  static void require_admin();
  static vector<User *> get_all_users();
};

string roleToString(UserRole role);
