/* Stanley Barnes
 * wbarne12
 * 000-67-7926
 * The Fraction class manages a fraction, where the numerator and 
 * the denominator are both products of positive integers.  
 * 
 * Help recieved:
 *   Finding next element in set:
 *   https://cplusplus.com/reference/iterator/next
 */
#include <set>
#include <iostream>
#include "fraction.hpp"
using namespace std;

// Clear both the numerator and denominator
void Fraction::Clear()
{
  numerator.clear();
  denominator.clear();
}

// Add a number to the numerator
bool Fraction::Multiply_Number(int n) {
  if (n < 1)  return false;
  if (n == 1) return true;

  multiset<int>::const_iterator nit;
  multiset<int>::const_iterator dit;

  numerator.insert(n);

  nit = numerator.find(n);
  dit = denominator.find(n);

  // If programs fails to find "n", then the iterator will equal the ending node. 
  if(nit != numerator.end() && dit != denominator.end()) {
    numerator.erase(nit);
    denominator.erase(dit);
  }

  return true;
}

// Add a number to the denominator
bool Fraction::Divide_Number(int n) { 
  if (n < 1)  return false;
  if (n == 1) return true;

  multiset<int>::const_iterator nit;
  multiset<int>::const_iterator dit;

  denominator.insert(n);

  nit = numerator.find(n);
  dit = denominator.find(n);
  
  // If n exists in the numerator, remove both numbers
  if(nit != numerator.end() && dit != denominator.end()) {
    numerator.erase(nit);
    denominator.erase(dit);
  }

  return true;
}

// Add the numbers 2 through n to the numerator
bool Fraction::Multiply_Factorial(int n) {
  if (n < 1)  return false;
  if (n == 1) return true;

  // Multiply by n(n-1)(n-2)...(n==2).
  for (int i = 2; i <= n; i++) {
    Multiply_Number(i);
  }

  return true;
}     

// Add the numbers 2 through n to the denominator
bool Fraction::Divide_Factorial(int n) {
  if (n < 1)  return false;
  if (n == 1) return true;

  // Divide by n(n-1)(n-2)...(n==2).
  for (int i = 2; i <= n; i++) {
    Divide_Number(i);
  }

  return true;
}

// Effect multiplying by n-choose-k
bool Fraction::Multiply_Binom(int n, int k) {
  /*     n!     The numerator can be represened be done by dividing by k
   *   ------   resulting in the numerator having n(n-1)(n-2)...(n==k). 
   *   k!(n-k)! This cancels out "k!" to have "(n-k)!"
   */

  if (n < 0)  return false;
  if (n == 0) return true;
  if (k < 0)  return false;
  if (k == 0) return true;

  for (int i = n; i > k; i--) {
    Multiply_Number(i);
  } 
  for (int i = 1; i < (n-k); i++) {
    Divide_Number(i+1);
  }

  return true;
}
 
// Effect dividing by n-choose-k
bool Fraction::Divide_Binom(int n, int k) {
  if (n < 0)  return false;
  if (n == 0) return true;
  if (k < 0)  return false;
  if (k == 0) return true;

  for (int i = n; i > k; i--) {
    Divide_Number(i);
  }
  for (int i = 1; i < (n-k); i++) {
    Multiply_Number(i+1);
  }

  return true;
}

// Swap the numerator and denominator
void Fraction::Invert(){
  swap(numerator, denominator);
}

// Print the equation for the fraction (see the lab writeup)
void Fraction::Print() const 
{
  multiset<int>::const_iterator it;

  if (numerator.empty()) cout << 1;

  for (it = numerator.begin(); it != numerator.end(); it++) {
    if (next(it) == numerator.end()) {
      cout << *it;
    } else {
      cout << *it << " * ";
    }
  }

  if (denominator.empty()) {
    cout << endl;
  } else {
    cout << " / ";
  }

  for (it = denominator.begin(); it != denominator.end(); it++) {
    if (next(it) == denominator.end()) {
      cout << *it << endl;
    } else {
      cout << *it << " / ";
    }
  }
}

// Calculate the product as a double.
double Fraction::Calculate_Product() const
{
  long double num = 1;
  multiset<int>::const_iterator it;

  for (it = numerator.begin(); it != numerator.end(); it++) {
    num *= *it;
  }
  for (it = denominator.begin(); it != denominator.end(); it++) {
    num /= *it;
  }

  return num;
}