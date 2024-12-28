/* Stanley Barnes
 * wbarne12
 * 000-65-7926
 * This program defines the methods in the Pgm class.
*/

#include "pgm.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

bool Pgm::Read(const std::string &file)
{
  ifstream fin;
  string s;
  size_t i, j, r, c, v;

  fin.open(file.c_str());
  if (fin.fail()) return false;
  
  if (!(fin >> s)) return false;
  if (s != "P2") return false;
  if (!(fin >> c) || c <= 0) return false;
  if (!(fin >> r) || r <= 0) return false;
  if (!(fin >> i) || i != 255) return false;

  Pixels.resize(r);
  for (i = 0; i < r; i++) {
    Pixels[i].clear();
    for (j = 0; j < c; j++) {
      if (!(fin >> v) || v > 255) return false;
      Pixels[i].push_back(v);
    }
  }
  if (fin >> s) return false;
  fin.close();
  return true;
}

bool Pgm::Write(const std::string &file) const 
{
  ofstream fout(file);
  if (fout.fail()) return false;
  if (Pixels.empty()) return false;

  // Header
  fout << "P2" << endl;
  fout << Pixels[0].size() << " " << Pixels.size() << endl;
  fout << 255 << endl;

  // Prints out 20 pixles, and then goes to next line
  // No space after last pixel in a line
  int counter = 0;
  size_t final_index = Pixels.size() * Pixels[0].size() - 1;

  for (size_t i = 0; i < Pixels.size(); i++) {
    for (size_t g = 0; g < Pixels[i].size(); g++) {
      if (counter == 20) {
        fout << endl;
        counter = 0;
      }

      fout << Pixels[i][g];

      if ((counter < 19) && ((i+1) * (g+1) - 1 != final_index)) {
        fout << " ";
      }
      
      counter++;
    }
  }

  fout << endl;
  fout.close();

  return true;
};

bool Pgm::Create(size_t r, size_t c, size_t pv)
{
  if (r < 1 || c < 1 || pv > 255) {
    return false;
  }

  Pixels.resize(r);
  for (size_t i = 0; i < Pixels.size(); i++) {
    Pixels[i].resize(c, pv);
  }

  return true;
}

bool Pgm::Clockwise() 
{
  // For a clockwise rotation, the rows become columns and columns become rows.
  const int rows = Pixels.size();
  const int columns = Pixels[0].size();

  vector < vector<int> > newPixels(columns, vector<int> (rows));
  
  for (int i = 0; i < rows; i++) {
    for (int g = 0; g < columns; g++) {     
      newPixels[g][rows - 1 - i] = Pixels[i][g];
    }
  }

  Pixels = newPixels;

  return true;
}

bool Pgm::Cclockwise()
{
  // A CCW rotation is the same a 3 CW rotations.
  for (int i = 0; i < 3; i++) {
    Clockwise();
  }

  return true;
}

bool Pgm::Pad(size_t w, size_t pv)
{
  if (pv > 255) return false;

  const int rows = Pixels.size();
  const int columns = Pixels[0].size();
  const int w2 = w * 2; // twice the width

  // Making a pgm that has `width + 2 * padding` and `height + 2 * padding`.
  // All values in this vector are pv
  vector < vector<int> > paddedPixels(rows + w2, vector<int> (columns + w2, pv));

  // Replacing the pixels in the padding vector with the original vector.
  for (int i = 0; i < rows; i++) {
    for (int g = 0; g < columns; g++) {
      paddedPixels[w + i][w + g] = Pixels[i][g];
    }
  }

  Pixels = paddedPixels;

  return true;
}

bool Pgm::Panel(size_t r, size_t c) 
{
  // Make a panelPixel vector that does NOT have any columns initalized.
  // Outter loop: Repeats a strip of panels.
  // Middle loop: Increments inner loop to go to the next row. 
  // Inner  loop: Takes the g row of pixels and repeats it c times. 

  if (r == 0 || c == 0) return false;

  vector < vector <int> > panelPixel(r * Pixels.size());

  for (size_t i = 0; i < r; i++) {
    for (size_t g = 0; g < Pixels.size(); g++) {
      for (size_t k = 0; k < c; k++) {
        panelPixel[g + (i * Pixels.size())].insert(
          panelPixel[g + (i * Pixels.size())].end(), 
          Pixels[g].begin(), 
          Pixels[g].end());
      }
    }
  }

  Pixels = panelPixel;

  return true;
}

bool Pgm::Crop(size_t r, size_t c, size_t rows, size_t cols) 
{
  if (rows+r > Pixels.size() || cols+c > Pixels[0].size()) return false;

  //Grabs a cropped row starting at Pixels[r] and ends at Pixels[r+rows]
  vector < vector<int> > croppedPixels(rows);

  for (size_t i = 0; i < rows; i++) {
    vector <int> croppedRow(Pixels[i+r].begin() + c, Pixels[i+r].begin() + cols+c);
    croppedPixels[i] = croppedRow;
  }

  Pixels = croppedPixels;

  return true;
}
