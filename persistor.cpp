#include "persistor.hpp"

void Persistor::save(string table_name, vector<string> row)
{
  string filename = "data/" + table_name + ".txt";
  // Open the file in append mode
  ofstream out(filename, ios::app);

  if (!out.is_open())
    throw "Error: Could not open file " + filename + " for writing.";

  // Write row data to the file
  for (const string &value : row)
  {
    // check if the value reference is equal to the last element of the vector
    out << value << (value == row.back() ? "\n" : ",");
  }

  // Close the file
  out.close();
}

vector<vector<string>> Persistor::get_table(string table_name)
{
  vector<vector<string>> table_data;
  string filename = "data/" + table_name + ".txt";

  // Open the file in read mode
  ifstream in(filename);

  if (!in.is_open())
  {
    throw "Error: Could not open file " + filename + " for reading.";
  }

  string line;
  while (getline(in, line))
  {
    vector<string> row = split_into_vector(line, ',');
    table_data.push_back(row);
  }

  // Close the file
  in.close();

  return table_data;
}
