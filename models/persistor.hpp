#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "../utilities/utils.hpp"
using namespace std;

class Persistor
{
public:
  static void save(string table_name, vector<string> row);
  static vector<vector<string>> get_table(string table_name);
};
