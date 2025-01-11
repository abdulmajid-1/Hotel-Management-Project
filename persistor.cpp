#include "persistor.hpp"

void Persister::save(string table_name, vector<string> row)
{
  string filename = table_name + ".txt";
  // Open the file in append mode
  ofstream out(filename, ios::app);

  if (!out.is_open())
  {
    cout << "Error: Could not open file " << filename << " for writing." << endl;
    return;
  }

  // Write row data to the file
  for (const string &value : row)
  {
    // check if the value reference is equal to the last element of the vector
    out << value << (value == row.back() ? "\n" : ", ");
  }

  // Close the file
  out.close();
}
