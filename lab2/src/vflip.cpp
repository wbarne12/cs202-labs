/*
 * This program rotates the image around the x-axis.
 */

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

void printHeader(int c, int r) {
  cout << "P2" << endl;
  cout << c << " " << r << endl;
  cout << "255" << endl;
}

int main() {
  int rows = 0, columns = 0, brightness = 0, midpoint = 0;
  string line;
  vector<int> pixels;

  // Checking to see if file has P2.
  cin >> line;
  if (line != "P2") {
    cerr << "Bad PGM file -- first word is not P2" << endl;
    return 1;
  }

  // Getting the rows and columns.
  cin >> columns >> rows;
  if (rows < 1) {
    cerr << "Bad PGM file -- No row specification" << endl;
    return 1;
  } else if (columns < 1) {
    cerr << "Bad PGM file -- No column specification" << endl;
    return 1;
  }

  // Checking for 255.
  cin >> brightness;
  if (brightness != 255) {
    cerr << "Bad PGM file -- No 255 following the rows and columns" << endl;
    return 1;
  }

  // Storing all the pixels and making sure they are not out of range.
  for (int i = 0; i < (rows * columns); i++) {
    int num = 0;
    cin >> num;
    if (num < 0 || num > 255) {
      cerr << "Bad PGM file -- pixel " << i << " is not a number between 0 and 255" << endl;
      return 1;
    }
    pixels.push_back(num);
  }

  // If there is anything in line, then there is extra stuff.
  cin >> line;
  if (!cin.eof()) {
    cerr << "Bad PGM file -- Extra stuff after the pixels" << endl;
    return 1;
  }


  // Current square only loops through the "positive" x-axis,
  // while x_reflection loops through the "negative" x-axis.
  midpoint = rows / 2;
  for (int i = 0; i < columns; i++) {
    for (int g = 0; g < midpoint; g++) {
        const int current_square = (columns * g) + i;  
        const int x_reflection = (columns * (rows - g - 1)) + i; 
        int temp = pixels[current_square];
        pixels[current_square] = pixels[x_reflection];
        pixels[x_reflection] = temp;
    }
  }

  // Printing.
  printHeader(columns, rows);
  for (size_t i = 0; i < pixels.size(); i++) {
    cout << pixels[i] << endl;
  }

  return 0;
}
