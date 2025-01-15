#include "utils.hpp"

vector<string> split_into_vector(string str, char separator)
{
  vector<string> arr;
  string temp = "";
  for (int i = 0; i < str.length(); i++)
  {
    if (str[i] == separator)
    {
      arr.push_back(temp);
      temp = "";
    }
    else
    {
      temp += str[i];
    }
  }
  arr.push_back(temp);
  return arr;
}

string joins(vector<string> arr, string separator)
{
  string str = "";
  for (int i = 0; i < arr.size(); i++)
  {
    str += arr[i];
    if (i < arr.size() - 1)
    {
      str += separator;
    }
  }
  return str;
}

void clear()
{
  // if the operating system is windows
// then run this
#ifdef _WIN32
  system("cls");
#else
  // if operating system is unix or linux, run this
  system("clear");
#endif
}

string to_lower_str(string str)
{
  string answer = "";
  for (int i = 0; str[i] != '\0'; i++)
  {
    if (str[i] >= 65 && str[i] <= 92)
    {
      answer += (char)(str[i] + 32);
    }
    else
    {
      answer += str[i];
    }
  }
  return answer;
}

string to_upper_str(string str)
{
  string answer = "";
  for (int i = 0; str[i] != '\0'; i++)
  {
    if (str[i] >= 97 && str[i] <= 122)
    {
      answer += (char)(str[i] - 32);
    }
    else
    {
      answer += str[i];
    }
  }
  return answer;
}

bool str_includes(const string &str, const string &substr)
{
  if (substr.empty())
  {
    return true;
  }
  if (str.size() < substr.size())
  {
    return false;
  }

  for (int i = 0; i <= str.size() - substr.size(); ++i)
  {
    int j = 0;
    while (j < substr.size() && str[i + j] == substr[j])
    {
      ++j;
    }
    if (j == substr.size())
    {
      return true;
    }
  }
  return false;
}

void show_as_table(vector<vector<string>> matrix)
{
  int rows = matrix.size();
  int cols = matrix[0].size();

  // Calculate the maximum length of strings in each column
  int max_col_lengths[cols];
  for (int i = 0; i < cols; i++)
  {
    max_col_lengths[i] = 0;
    for (int j = 0; j < rows; j++)
    {
      int len = matrix[j][i].length();
      if (len > max_col_lengths[i])
      {
        max_col_lengths[i] = len;
      }
    }
  }

  // Display the table
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      cout << "+-";
      for (int k = 1; k <= max_col_lengths[j]; k++)
      {
        cout << "-";
      }
      cout << "-";
    }
    cout << "+" << endl;

    for (int j = 0; j < cols; j++)
    {
      cout << "| " << matrix[i][j];
      for (int k = 1; k <= max_col_lengths[j] - (matrix[i][j]).length(); k++)
      {
        cout << ' ';
      }
      cout << " ";
    }
    cout << "|" << endl;
  }

  // Bottom border of the table
  for (int j = 0; j < cols; j++)
  {
    cout << "+-";
    for (int k = 1; k <= max_col_lengths[j]; k++)
    {
      cout << '-';
    }
    cout << "-";
  }
  cout << "+" << endl;
}
