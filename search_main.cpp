#include <iostream>
#include <string>

#include "include/search.h"

using namespace std;

int main() {
  string filename;
  cin >> filename;

  cout << "Enter a filename: ";

  searchEngine(filename);
  return 0;
}
