/* Stanley Barnes
 * wbarne12
 * 000-67-7926
 * Uses the fraction class to find the probabilty of gambling. 
 */
#include <vector>
#include <iomanip>
#include <iostream>
#include "fraction.hpp"
using namespace std;

int main() {
  // Variables and stuff.
  Fraction fraction;
  double bet = 0, num2 = 0, probability = 0, expected_return = 0;
  int num_balls = 0, num1 = 0;
  vector<int> nums;
  vector<double> payout;

  // First two numbers are the bet and num of balls
  cin >> bet >> num_balls;
  double return_per_bet = -bet;

  // Read in payouts for number of balls.
  while (cin >> num1 >> num2) {
    nums.push_back(num1);
    payout.push_back(num2);
  }

  // Header. 
  printf("Bet: %.2f\n", bet);
  printf("Balls Picked: %i\n", num_balls);

  for (size_t i = 0; i < nums.size(); i++) {
    const int p = num_balls;
    const int c = nums[i];
    const double last = payout[i];

    // Find the probability
    fraction.Clear();
    fraction.Multiply_Binom(80-p, 20-c);
    fraction.Multiply_Binom(p, c);
    fraction.Divide_Binom(80, 20);
    probability = fraction.Calculate_Product();

    expected_return = (probability * last);
    return_per_bet += expected_return;
    
    // Body.
    cout << "  Probability of catching " << c << " of " << p << ": " 
         << probability << " -- " << "Expected return: " << expected_return << endl;
  }

  // Footer.
  if (return_per_bet < 0 && return_per_bet > -0.005) {
    return_per_bet = -return_per_bet;
  }
  cout << setprecision(2) << fixed;
  cout << "Your return per bet: " << return_per_bet << endl;
  cout << "Normalized: " << return_per_bet / bet << endl;

  return 0;
}