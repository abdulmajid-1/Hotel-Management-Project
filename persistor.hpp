#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "utils.hpp"
using namespace std;

class Persister
{
public:
  static void save(string table_name, vector<string> row);
};
