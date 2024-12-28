/* 
 * This program verifies that a pgm file is valid and outputs info.
 */

#include <iostream>
#include <iomanip>
using namespace std;


int main() {
  int row = 0, column = 0, brightness = 0, countPixels = 0, valueTotal = 0;
  double average = 0;
  string line;

  // Checking to see if file has P2.
  cin >> line;
  if (line != "P2") {
    cerr << "Bad PGM file -- first word is not P2" << endl;
    return 1;
  }

  // Getting the row and column.
  cin >> column >> row;
  if (row < 1) {
    cerr << "Bad PGM file -- No row specification" << endl;
    return 1;
  } else if (column < 1) {
    cerr << "Bad PGM file -- No column specification" << endl;
    return 1;
  }

  // Checking for 255.
  cin >> brightness;
  if (brightness != 255) {
    cerr << "Bad PGM file -- Brightness is not correct" << endl;
    return 1;
  }

  // Finding total number of pixels and seeing if any are out of range.
  for (int i = 0; i < (row * column); i++) {
    int num = 0;
    cin >> num;
    if (num < 0 || num > 255) {
      cerr << "Bad PGM file -- pixel " << i << " is not a number between 0 and 255" << endl;
      return 1;
    }
    valueTotal += num;
    countPixels++;
  }

  // If there is anything in line, then there is extra stuff.
  cin >> line;
  if (!cin.eof()) {
    cerr << "Bad PGM file -- Extra stuff after the pixels" << endl;
    return 1;
  }

  // Finding the average and printing the results.
  average = (valueTotal * 1.0 / (row * column));

  cout << right << setprecision(3) << fixed;
  cout << "# Rows:"    << setw(12) << row         << endl <<
          "# Columns:" << setw(9)  << column      << endl <<
          "# Pixels:"  << setw(10) << countPixels << endl <<
          "Avg Pixel:" << setw(9)  << average     << endl;
  
  return 0;
}
