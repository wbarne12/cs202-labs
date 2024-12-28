/* Stanley Barnes
 * wbarne12
 * 000-67-7926
 * 
 * The code_processor is a mock of processing prizes for users.
 * More information on methods in include/code_processor.hpp.
 * 
 * Lab Write-up:
 *   https://web.eecs.utk.edu/~jplank/plank/classes/cs202/Labs/Lab7/ */
#include <set>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "code_processor.hpp"
using namespace std;

unsigned int djb_hash(const string &s)
{
  size_t i;
  unsigned int h;

  h = 5381;

  for (i = 0; i < s.size(); i++) {
    h = (h << 5) + h + s[i];
  }
  return h;
}

bool Code_Processor::New_Prize(const string &id, const string &description, int points, int quantity) {
  // Prize id already exists
  if (Prizes.find(id) != Prizes.end()) return false;
  if (points <= 0) return false;
  if (quantity <= 0) return false;

  Prize *new_prize = new Prize;
  new_prize->id = id;
  new_prize->description = description;
  new_prize->points = points;
  new_prize->quantity = quantity;

  Prizes.insert(make_pair(id, new_prize));

  return true;
}

bool Code_Processor::New_User(const string &username, const string &realname, int starting_points) {
  // If user already exists
  if (Names.find(username) != Names.end()) return false;
  if (starting_points < 0) return false;

  User *new_user = new User;
  new_user->username = username;
  new_user->realname = realname;
  new_user->points = starting_points;
  
  Names.insert(make_pair(username, new_user));

  return true;
}

bool Code_Processor::Delete_User(const string &username) {
 
  unordered_map <string, User*>::const_iterator nit; // Names.
  set <string>::const_iterator pit; // Phone nums.
  nit = Names.find(username);

  // If user DNE
  if (nit == Names.end()) return false;

  // Deleting all phone map entries from Phone.
  for (pit = nit->second->phone_numbers.begin(); pit != nit->second->phone_numbers.end(); pit++) {
    Phones.erase(*pit);
  }
  
  delete nit->second;
  Names.erase(nit);

  return true;
}


bool Code_Processor::Add_Phone(const string &username, const string &phone) {

  unordered_map <string, User*>::const_iterator nit;
  unordered_map <string, User*>::const_iterator pit;
  nit = Names.find(username);
  pit = Phones.find(phone);

  /* If user DNE
   * If phone already exists. */
  if (nit == Names.end()) return false;
  if (pit != Phones.end()) return false;

  nit->second->phone_numbers.insert(phone);
  Phones.insert(make_pair(phone, nit->second));

  return true;
}

bool Code_Processor::Remove_Phone(const string &username, const string &phone) {

  unordered_map <string, User*>::const_iterator nit;
  unordered_map <string, User*>::const_iterator pit;
  nit = Names.find(username);
  pit = Phones.find(phone);

  /* If user is not found.
   * If no phone number is found.
   * If phone number is registered to different user. */
  if (nit == Names.end()) return false;
  if (pit == Phones.end()) return false;
  if (nit->second->username != pit->second->username) return false;

  nit->second->phone_numbers.erase(phone);
  Phones.erase(phone);

  return true;
}

string Code_Processor::Show_Phones(const string &username) const {

  unordered_map <string, User *>::const_iterator it;
  set <string>::const_iterator sit;
  string result;
  it = Names.find(username);

  // If no user is found
  if (it == Names.end()) return string("BAD USER");

  for (sit = it->second->phone_numbers.begin(); sit != it->second->phone_numbers.end(); sit++) {
    result += *sit + "\n";
  }

  return result;
}


int Code_Processor::Enter_Code(const string &username, const string &code) {
  
  unordered_map <string, User *>::const_iterator nit; // Names
  unordered_set <string>::const_iterator cit; // Codes
  nit = Names.find(username);
  cit = Codes.find(code);
  unsigned int hashed_code = djb_hash(code);

  /* 1. If prize has been entered before.
   * 2. If user DNE.
   * 3. If code is not divisible by 13 or 17. */
  if (cit != Codes.end()) return -1;
  if (nit == Names.end()) return -1;
  if (hashed_code % 17 != 0 && hashed_code % 13 != 0) return 0;

  if (hashed_code % 17 == 0) {
    nit->second->points += 10;
    Codes.insert(code);
    return 10;
  } 
  else if (hashed_code % 13 == 0) {
    nit->second->points += 3;
    Codes.insert(code);
    return 3;
  }

  return -1;
}

int Code_Processor::Text_Code(const string &phone, const string &code) {
  
  unordered_map <string, User *>::const_iterator pit; // Phones
  unordered_set <string>::const_iterator cit; // Codes
  pit = Phones.find(phone);
  cit = Codes.find(code);
  unsigned int hashed_code = djb_hash(code);

  /* 1. If phone DNE.
   * 2. If prize has been entered before.
   * 3. If code is not divisible by 13 or 17. */
  if (pit == Phones.end()) return -1;
  if (cit != Codes.end()) return -1;
  if (hashed_code % 17 != 0 && hashed_code % 13 != 0) return 0;

  if (hashed_code % 17 == 0) {
    pit->second->points += 10;
    Codes.insert(code);
    return 10;
  } 
  else if (hashed_code % 13 == 0) {
    pit->second->points += 3;
    Codes.insert(code);
    return 3;
  }

  return -1;
}

bool Code_Processor::Mark_Code_Used(const string &code) {
  unsigned int hashed_code = djb_hash(code);

  if (hashed_code % 17 != 0 && hashed_code % 13 != 0) return false;
  if (Codes.find(code) != Codes.end()) return false;

  Codes.insert(code);

  return true;
}


int Code_Processor::Balance(const string &username) const {
  
  unordered_map <string, User *>::const_iterator it;
  it = Names.find(username);

  // If the user does not exist
  if (it == Names.end()) return -1;

  const int balance = it->second->points;
  
  return balance;
}

bool Code_Processor::Redeem_Prize(const string &username, const string &prize) {
  
  unordered_map <string, User *>::const_iterator nit; // Names
  unordered_map <string, Prize *>::const_iterator pit; // Prizes
  nit = Names.find(username);
  pit = Prizes.find(prize);

  /* 1. User DNE.
   * 2. Prize DNE.
   * 3. User does nto have enough points. */
  if (nit == Names.end()) return false;
  if (pit == Prizes.end()) return false;
  if (nit->second->points < pit->second->points) return false;

  nit->second->points -= pit->second->points;
  pit->second->quantity--;

  if (pit->second->quantity == 0) {
    delete pit->second;
    Prizes.erase(prize);
  }

  return true;
}


Code_Processor::~Code_Processor() {
  unordered_map <string, User *>::const_iterator nit; // Names
  unordered_map <string, Prize *>::const_iterator pit; // Prizes

  for (nit = Names.begin(); nit != Names.end(); nit++) {
    delete nit->second;
  }
  for (pit = Prizes.begin(); pit != Prizes.end(); pit++) {
    delete pit->second;
  }
}

bool Code_Processor::Write(const string &filename) const {
  
  // If filesteam fails to open.
  ofstream fout(filename);
  if (! fout.is_open()) return false;

  // Iterators
  unordered_map <string, User *>::const_iterator nit; // Names and Phones iterator.
  unordered_map <string, Prize *>::const_iterator pit; // Prize iterator.
  unordered_set <string>::const_iterator cit; // Code iterator.
  
  //Writing prizes
  for (pit = Prizes.begin(); pit != Prizes.end(); pit++) {
    const string id = pit->second->id;
    const string description = pit->second->description;
    const int points = pit->second->points;
    const int quantity = pit->second->quantity;

    fout << left;
    fout << "PRIZE " << id << " "  << points << " " << quantity << " " << description << endl;
  }
  // Writing users
  for (nit = Names.begin(); nit != Names.end(); nit++) {
    const string username = nit->second->username;
    const string real_name = nit->second->realname;
    const int points = nit->second->points;
  
    fout << "ADD_USER " << username << " " << points << " " << real_name << endl;
  }
  // Writing phone numbers
  for (nit = Phones.begin(); nit != Phones.end(); nit++) {
    const string phone_num = nit->first;
    const string username = nit->second->username;

    fout << "ADD_PHONE " << username << " " << phone_num << endl;
  }
  // Writing codes
  int counter = 0;
  for (cit = Codes.begin(); cit != Codes.end(); cit++, counter++) {
    if (counter % 8 == 0) fout << "MARK_USED ";

    fout << *cit << " ";

    if ((counter + 1) % 8 == 0) fout << endl;
  }
  if (counter % 8 != 0) fout << endl;
 
  
  fout.close();
  return true;
}
