/*
 * This program makes a checkerboard of size (R*C)
 */
#include <iostream>
#include <string>
using namespace std;

int main() {
  // make the variables :)
  int row = 0, column = 0, cycleSize = 0, width = 0;
  char startingChar;
  
  // input and error checking
  cin >> row;
  cin >> column;
  cin >> startingChar;
  cin >> cycleSize;
  cin >> width;
  if (cin.fail()) {
    cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W" << endl;
    cin.clear();
    return 1;
  }

  // if char goes out of bounds
  if ((startingChar + cycleSize) > 127) {
    return 0;
  }
  // if row or column is 0, no need for output
  if (row == 0 || column == 0) {
    return 0;
  }


  // SC + (r+c)%CS to find the char at i,k position
  // the g loop is to make sure that the checker board has the right width
  for (int i = 0; i < row; i++) {
    for (int g = 0; g < width; g++) {
      for (int k = 0; k < column; k++) {
        char temp = (startingChar + (i + k) % cycleSize);
        string tempStr(width, temp);
        cout << tempStr;
      }
    cout << endl;
    }
  }
  
}
