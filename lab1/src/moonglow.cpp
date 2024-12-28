/* 
 * This program makes the moon glow.
 */

#include <iostream>
#include <cmath>
using namespace std;

double getAverage() {
  double number = 0, total = 0;
  int count = 0;

  // break out of reading numbers when a string is read
  while (cin >> number) {
    if (cin.fail()) {
      cin.clear();
      break;
    }
    total += number;
    count ++;
  }
  // making sure i don't divide by zero if count == 0
  return (count > 0) ? (total / count) : 0;
}

double getOnlyNums(string &line) {
  double num = 0;

  try {
    double temp = stod(line); // hello
    // if the string contains "inf" then it registers that num is infinity
    if (! isinf(temp)) {
      num += temp;
    } 
  } 
  // clear cin if a string that isn't "NAME" or "AVERAGE"
  catch(invalid_argument& e) {
    cin.clear();
  } 
   
  return num;
}


int main() {
  string line, name;
  double score = 0;

  while (cin >> line) {
    // getting the name
    if (line == "NAME") {
      cin >> line;
      name = line;
    }

    // finding the average
    else if (line == "AVERAGE") {
      score += getAverage();
      cin.clear();
    }

    else {
      score += getOnlyNums(line);
    }
  }
  cout << name << " " << score << endl;
}