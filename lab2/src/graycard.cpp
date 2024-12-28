/*
 * This program makes a gray card.
 */
#include <iostream>
using namespace std;

void printHeader(int c, int r) {
  cout << "P2" << endl;
  cout << c << " " << r << endl;
  cout << "255" << endl;
}

int main(int argc, char* argv[]) {
  int row = 0, column = 0;

  if (argc != 3) {
    cerr << "usage: graycard rows cols" << endl;
    return 1;
  }
  try {
    row = stoi(argv[1]);
    column = stoi(argv[2]);
  } catch (invalid_argument &e) {
    cerr << "usage: graycard rows cols" << endl;
    return 1;
  }

  int thirdOfRow = row / 3;

  // Error checking.
  if (row < 1) {
    cerr << "usage: graycard rows cols" << endl;
    return 1;
  } 
  if (column < 1) {
    cerr << "usage: graycard rows cols" << endl;
    return 1;
  } 
  if (row % 3 != 0) {
    cerr << "rows must be a multiple of three" << endl;
    return 1;
  }

  // Printing.
  printHeader(column, row);

  // Outputs black, gray, and white for the gray card.
  for (int i = 0; i < column; i++) {
    for (int g = 0; g < thirdOfRow; g++) {
      cout << "0" << endl;
    }
  }
  for (int i = 0; i < column; i++) {
    for (int g = 0; g < thirdOfRow; g++) {
      cout << "209" << endl;
    }
  }
  for (int i = 0; i < column; i++) {
    for (int g = 0; g < thirdOfRow; g++) {
      cout << "255" << endl;
    }
  }

  return 0;
}
