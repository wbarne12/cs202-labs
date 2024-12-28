/* 
 * This program takes everything from the past 4 labs to make an evil program.
 * More detail about methods in bitmatrix.hpp and 
 *  https://web.eecs.utk.edu/~jplank/plank/classes/cs202/Labs/Lab5/.
 * 
 * Help recieved:
 *  Vector constructor:
 *    https://cplusplus.com/reference/vector/vector/vector/
 *  TA Yousif on Invert Function:
 *    https://dashboard.ticketsbot.net/manage/1278539760410951784/transcripts/view/339
 *  DJB Hash:
 *    https://web.eecs.utk.edu/~jplank/plank/classes/cs202/Notes/Hashing/index.html
 */

#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include <string>
#include <iostream>
#include "bitmatrix.hpp"
using namespace std;

// Makes an empty bitmaxtix of size row * column.
Bitmatrix::Bitmatrix(int rows, int cols)
{

  if (rows <= 0) throw string("Bad rows");
  if (cols <= 0) throw string("Bad cols");

  M.resize(rows);

  string zero_line(cols, '0');
  for (int i = 0; i < rows; i++) {
    M[i] = zero_line;
  } 
}
  
// Reads in a matrix. Cannot use Cols() and Rows() because they have not been asigned a value yet. 
Bitmatrix::Bitmatrix(const string &fn)
{
  ifstream fin;
  string line, result;
  size_t rows = 0;

  // Error checking.
  fin.open(fn);
  if (! fin.is_open()) throw string("Can't open file");

  // Stores the file in M without any white spaces.
  while(getline(fin, line)) {
    if (line.empty()) continue;
    result = "";

    for (size_t i = 0; i < line.size(); i++) {
      if (line[i] != ' ') result += line[i];
    }

    M.push_back(result);
    rows++;
  }

  // Making sure values are not out of range.
  for (size_t i = 1; i < M.size(); i++) {
    if (M[i-1].size() != M[i].size()) throw string("Bad file format");
  }

  fin.close();
}

Bitmatrix *Bitmatrix::Copy() const
{
  Bitmatrix* copy = new Bitmatrix(Rows(), Cols());

  for (size_t i = 0; i < M.size(); i++) {
    copy->M[i] = M[i];
  }

  return copy;
}

// Write to a file given name 'fn'.
bool Bitmatrix::Write(const string &fn) const
{
  ofstream fout;

  fout.open(fn);
  if (! fout.is_open()) return false;

  for (int i = 0; i < Rows(); i++) {
    fout << M[i] << endl;
  }

  return true;
}
 
// Prints out the vector with 'w' spacing and 'w' line breaks. 
void Bitmatrix::Print(size_t w) const
{
  for (int i = 0; i < Rows(); i++) {
    for (int g = 0; g < Cols(); g++) {
      cout << M[i][g];
      if (w > 0 && (g+1) % w == 0 && g < Cols() - 1) cout << " ";
    }
    cout << endl;
    if (w > 0 && (i+1) % w == 0 && i < Rows() - 1) cout << endl;
  }
}
                 
// Makes a pgm with each bit being a p by p square with 
bool Bitmatrix::PGM(const string &fn, int p, int border) const
{
  // Error checking.
  if (p < 0) return false;
  if (border < 0) return false;
  
  ofstream fout;
  fout.open(fn);
  if (! fout.is_open()) return false;

  const int rows = Rows();
  const int columns = Cols();
  
  // Header
  fout << "P2" << endl;
  fout << p*columns + border*(columns+1) << " " << p*rows + border*(rows+1) << endl;
  fout << 255 << endl;

  // Top border.
  for (int i = 0; i < border; i++) {
    for (int g = 0; g < p*columns + border*(columns+1); g++) {
      fout << "0 ";
    }
    fout << endl;
  }

  // Body
  for (size_t i = 0; i < M.size(); i++) {
    for (int r = 0; r < p; r++) {
      for (size_t g = 0; g < M[0].size(); g++) {
        // Leading border to pixel.
        for (int k = 0; k < border; k++) { 
          fout << "0 ";
        }
        // Pixels in p by p square.
        for (int k = 0; k < p; k++) {
          if (M[i][g] == '0') fout << "255 ";
          else if (M[i][g] == '1') fout << "100 ";
        }
      }
      // Last border column
      for (int k = 0; k < border; k++) { 
        fout << "0 ";
      }
      fout << endl;
    }

    // Footer. Same code as header. Need repeated because each row needs a bottom.
    for (int i = 0; i < border; i++) {
      for (int g = 0; g < p*columns + border*(columns+1); g++) {
        fout << "0 ";
      }
    fout << endl;
    }
  }
  
  return true;
}

// Returns number of rows.
int Bitmatrix::Rows() const
{
  return M.size();
}

// Returns number of columns.                         
int Bitmatrix::Cols() const
{
  return M[0].size();
}

// Return val at row, col.                
char Bitmatrix::Val(int row, int col) const
{
  int columns = Cols();
  int rows = Rows();

  if (row < 0    || col < 0)       return 'x';
  if (row > rows || col > columns) return 'x';
  
  return M[row][col];
}

// Set a single value to 0 or 1.
bool Bitmatrix::Set(int row, int col, char val)
{
  int columns = Cols();
  int rows = Rows();

  // Error checking.
  if (row < 0 || col < 0)          return false;
  if (row > rows || col > columns) return false;
  if (val != 0 && val != 1 && val != '0' && val != '1') return false;
  
  M[row][col] = val;
  return true;
}

// Swaps rows.       
bool Bitmatrix::Swap_Rows(int r1, int r2)
{
  if (r1 < 0 || r2 < 0) return false;
  if (r1 > Rows() || r2 > Rows()) return false;

  swap(M[r1], M[r2]);
  return true;
}
              
// M[r1] = M[r1] xor M[r2]
bool Bitmatrix::R1_Plus_Equals_R2(int r1, int r2)
{
  // Error checking.
  if (r1 < 0 || r2 < 0) return false;
  if (r1 > Rows() || r2 > Rows()) return false;

  string result = "";

  for (int i = 0; i < Cols(); i++) {
    if (M[r1][i] == M[r2][i]) result += '0';
    else                      result += '1';
  }

  M[r1] = result;

  return true;
}
      
// xor
Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2)
{
  if (a1->Rows() != a2->Rows() || a1->Cols() != a2->Cols()) return NULL;

  Bitmatrix *sum = new Bitmatrix(a1->Rows(), a1->Cols());

  for (int i = 0; i < sum->Rows(); i++) {
    for (int g = 0; g < sum->Cols(); g++) {
      char a1_pos = a1->Val(i, g);
      char a2_pos = a2->Val(i, g);
      char place;

      if (a1_pos == a2_pos) place = '0';
      else place = '1';

      sum->Set(i, g, place);
    }
  }
  
  return sum;
}

// AND then OR
Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2)
{
  if (a1->Cols() != a2->Rows()) return NULL;

  Bitmatrix *product = new Bitmatrix(a1->Rows(), a2->Cols());

  for (int i = 0; i < product->Rows(); i++) {
    for (int g = 0; g < product->Cols(); g++) {
      int result = 0; 

      for (int k = 0; k < a1->Cols(); k++) {
        char a1_place = a1->Val(i, k);
        char a2_place = a2->Val(k, g); 

        int num1 = 0, num2 = 0;
        if (a1_place == '1') num1 = 1;
        if (a2_place == '1') num2 = 1;

        result += (num1 * num2);
      }
      result %= 2;
      char place;

      if (result == 1) place = '1';
      else place = '0';

      product->Set(i, g, place);
    }
  }

  return product;
}

// Sub matrix consisting of rows from 'a1' specified by 'rows' vector.
Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector <int> &rows)
{
  if (rows.empty()) return NULL;

  Bitmatrix *sub = new Bitmatrix(rows.size(), a1->Cols());

  for (size_t i = 0; i < rows.size(); i++) {
    for (int g = 0; g < a1->Cols(); g++) {
      char place = a1->Val(rows[i], g);
      if (place == 'x') return NULL;

      sub->Set(i, g, place);
    }
  }

  return sub;
}

// Makes an inverted bitmatrix. Follows the notes on how to invert.
Bitmatrix *Inverse(const Bitmatrix *m)
{
  if (m->Rows() != m->Cols()) return NULL;

  Bitmatrix *mx  = new Bitmatrix(m->Rows(), m->Cols());
  Bitmatrix *inv = new Bitmatrix(m->Rows(), m->Cols());

  for (int i = 0; i < mx->Rows(); i++) {
    for (int g = 0; g < mx->Cols(); g++) {
      mx->Set(i, g, m->Val(i, g));
    }
    inv->Set(i, i, '1');
  }
  // Copy and identity matrix made


  // Making mx and upper right hand triangle
  for (int i = 0; i < mx->Rows(); i++) {
    if (mx->Val(i, i) != '1') {
      bool isInvertable = false;

      for (int g = i+1; g < mx->Rows(); g++) {
        if (mx->Val(g, i) == '1') {
          mx->Swap_Rows(i, g);
          inv->Swap_Rows(i, g);
          isInvertable = true;
          break;
        }
      }
      if (! isInvertable) return NULL;
    }
    for (int g = i+1; g < mx->Rows(); g++) {
      if (mx->Val(g, i) == '1') {
        mx->R1_Plus_Equals_R2(g, i);
        inv->R1_Plus_Equals_R2(g, i);
      }
    }
  }

  // Final pass
  for (int i = mx->Rows() - 1; i > -1; i--) {
    for (int g = i+1; g < mx->Rows(); g++) {
      if (mx->Val(i, g) == '1') {
        mx->R1_Plus_Equals_R2(i, g);
        inv->R1_Plus_Equals_R2(i, g);
      }
    }
  }

  return inv;
}


// Uses the hash funtion from J Plank notes.
unsigned int DJB_Hash(const string &s)
{
  unsigned int h = 5381;

  for (size_t i = 0; i < s.size(); i++) {
    h = (h << 5) + h + s[i];
  }
  return h;
}

// Makes a hash table of 'size'.
BM_Hash::BM_Hash(int size)
{
  if (size <= 0) throw ("Bad size");

  Table.resize(size);
}

// Hash table witu pointers to a bitmatrix.
bool BM_Hash::Store(const string &key, Bitmatrix *bm)
{
  unsigned int hashed_key = DJB_Hash(key);
  unsigned int index = hashed_key % Table.size();

  // Making sure the key we are adding does not already exist.
  for (size_t i = 0; i < Table[index].size(); i++) {
    if (Table[index][i].key == key) return false;
  }

  // Adding to the hash table.
  HTE newEntry;
  newEntry.key = key;
  newEntry.bm = bm;

  Table[index].push_back(newEntry);
  return true;
}

// Returns the assoicated bitmatix with the given key
Bitmatrix *BM_Hash::Recall(const string &key) const
{
  unsigned int hashed_key = DJB_Hash(key);
  unsigned int index = hashed_key % Table.size();

  for (size_t i = 0; i < Table[index].size(); i++) {
    if (Table[index][i].key == key) return Table[index][i].bm;
  }

  return NULL;
}

// Returns a vector will all HTE's that have been stored.  
vector <HTE> BM_Hash::All() const
{
  vector <HTE> entries;

  for (size_t i = 0; i < Table.size(); i++) {
    for (size_t g = 0; g < Table[i].size(); g++) {
      if (Table[i][g].key == "") continue;

      entries.push_back(Table[i][g]);
    }
  }
  
  return entries;
}
