#include <iostream>
#include <vector>
using namespace std;

void clear();

string to_lower_str(string str);

string to_upper_str(string str);

bool str_includes(const string &str, const string &substr);

string joins(vector<string> arr, string separator);

vector<string> split_into_vector(string str, char separator);

void show_as_table_vector(vector<vector<string>> matrix);
