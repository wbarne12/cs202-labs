/* 
 * This program defines the methods in the hash_202.hpp file.
 * All of the methods have more detail in the hash_202.hpp.
 * 
 * Help recieved:
 * - String Iterators: https://devdocs.io/cpp/header/string
 * - Stoi using hex: https://devdocs.io/cpp/string/basic_string/stol
 * - Discord help w/ double hash: 
 *   https://dashboard.ticketsbot.net/manage/1278539760410951784/transcripts/view/257
 */

#include "hash_202.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

/* Both hash funtions return the hash value.
 * This value has NOT been modified in any way.
 */
size_t XOR_Hash(string key) {
  // Any string smaller than seven can just be returned. 
  if (key.size() < 7) return stoi(key, 0, 16);

  // XORing values of the string together. 
  size_t hashValue = 0;
  size_t i = 0;

  for (i = 0; i < key.size() / 7; i++) {
    string sevenStr(key.begin() + 7*i, key.begin() + 7*(i+1));
    hashValue ^= stoi(sevenStr, 0, 16);
  }

  // If there are left over nums after groupings of seven,
  // run this code to find final value to XOR
  if (key.size() % 7 != 0) {
    string nString(key.begin() + 7*i, key.end());
    hashValue ^= stoi(nString, 0, 16);
  }

  return hashValue;
}

size_t Last7_Hash(string key) {
  // If smaller than seven, just return the number.
  if (key.size() < 7) return stoi(key, 0, 16);

  // Get the last 7 digits and return the number. 
  string lastSeven(key.end() - 7, key.end());

  return stoi(lastSeven, 0, 16);
}

string Hash_202::Set_Up(size_t table_size, const std::string &fxn, const std::string &collision)
{
  // Error checking.
  if (!Vals.empty())                                  return "Hash table already set up";
  if (table_size <= 0)                                return "Bad table size";
  if (fxn != "Last7" && fxn != "XOR")                 return "Bad hash function";
  if (collision != "Linear" && collision != "Double") return "Bad collision resolution strategy";

  // Setting the variables.
  if (fxn == "XOR") Fxn = 0;
  else Fxn = 1;
  if (collision == "Linear") Coll = 'L';
  else Coll = 'D';

  Keys.resize(table_size, "");
  Vals.resize(table_size, "");

  Nprobes = 0;

  return "";
}

string Hash_202::Add(const std::string &key, const std::string &val) 
{
  // Error checking.
  int emptySpaces = 0;
  bool hasKey = false;

  if (Vals.empty())     return "Hash table not set up";
  if (key == "")        return "Empty key"; 
  if (val == "")        return "Empty val";

  for (size_t i = 0; i < key.size(); i++) {
    if (!((key[i] >= '0' && key[i] <= '9') ||
      (key[i] >= 'a' && key[i] <= 'f') ||
      (key[i] >= 'A' && key[i] <= 'F'))) {
                        return "Bad key (not all hex digits)";
    }
  }
  for (size_t i = 0; i < Keys.size(); i++) {
    if (Keys[i] == "") emptySpaces++; 
    if (Keys[i] == key) hasKey = true;
  }
  if (emptySpaces == 0) return "Hash table full";
  if (hasKey)           return "Key already in the table";


  // Getting the hash values.
  size_t xor_hash   = XOR_Hash(key);
  size_t last7_hash = Last7_Hash(key);
  size_t og_hash = 0, opposite_hash = 0; 

  switch (Fxn) {
  case 0:
    og_hash = xor_hash;
    opposite_hash = last7_hash;
    break;
  case 1:
    og_hash = last7_hash;
    opposite_hash = xor_hash;
    break;
  }

  // Checking if opposite hash is zero and setting to one if it is.
  if (opposite_hash % Keys.size() == 0) opposite_hash = 1;

  // Collision resolution and adding the values to the vectors.
  switch (Coll) {
  case 'L':
    for (size_t i = 0; i < Keys.size(); i++) {
      size_t index = (og_hash + i) % Keys.size();
      if (Keys[index] != "") continue;

      Keys[index] = key;
      Vals[index] = val;
      Nkeys++;
      return "";
    }
    break;

  case 'D':
    for (size_t i = 0; i < Keys.size(); i++) {
      size_t double_index = (og_hash + i*opposite_hash) % Keys.size();
      if (Keys[double_index] != "") continue;

      Keys[double_index] = key;
      Vals[double_index] = val;
      Nkeys++;
      return "";
    }
    break;
  }

  return "Cannot insert key";
}

string Hash_202::Find(const std::string &key) 
{
  // Error checking.
  if (Keys.empty()) return "";
  for (size_t i = 0; i < key.size(); i++) {
    if (!((key[i] >= '0' && key[i] <= '9') ||
          (key[i] >= 'a' && key[i] <= 'f') ||
          (key[i] >= 'A' && key[i] <= 'F'))) {
        return "";
    }
}
 
  // Getting the hash values.
  size_t xor_hash   = XOR_Hash(key);
  size_t last7_hash = Last7_Hash(key);
  size_t og_hash = 0, opposite_hash = 0;

  // Collision resoultion. 
  switch (Fxn) {
  case 0:
    og_hash = xor_hash;
    opposite_hash = last7_hash;
    break;
  case 1:
    og_hash = last7_hash;
    opposite_hash = xor_hash;
    break;
  }

  // Checking if opposite hash is zero and setting to one if it is.
  if (opposite_hash % Keys.size() == 0) opposite_hash = 1;

  // Collision resolution and adding the values to the vectors.
  switch (Coll) {
  case 'L':
    for (size_t i = 0; i < Keys.size(); i++) {
      size_t index = (og_hash + i) % Keys.size();
      if (Keys[index] == key) {
        Nprobes += i; 
        return Vals[index];
      }
    }
    break;
  case 'D':
    for (size_t i = 0; i < Keys.size(); i++) {
      size_t double_index = (og_hash + i*opposite_hash) % Keys.size();
      if (Keys[double_index] == key) {
        Nprobes += i; 
        return Vals[double_index];
      }
    }
    break;
  }

  return "";
}

void Hash_202::Print() const
{
  cout << right;

  for (size_t i = 0; i < Keys.size(); i++) {
    if (Vals[i] == "") continue;
    cout << setw(5) << i << " " << Keys[i] << " " << Vals[i] << endl;
  }
}

size_t Hash_202::Total_Probes() 
{
  // Error checking.
  if (Vals.empty()) return 0;

  Nprobes = 0;

  for (size_t i = 0; i < Keys.size(); i++) {
    if (Keys[i] != "") {
      Find(Keys[i]);  
    }
  }

  return Nprobes;
}
