#include "largenum.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Your code goes here

// output number with a comma after ever 3 digits,
// e.g. 1234567890 -> 1,234,567,890
ostream &operator<<(ostream &out, const LargeNum &num) {
  string output;
  if (num.isNeg)
    output.append("-");
  int indexI = num.myNum.size() - 1;
  while (indexI >= 0) {
    output.push_back(num.myNum[indexI]);
    if ((indexI % 3 == 0 && num.myNum.size() > 3) && indexI != 0) {
      output.append(",");
    }
    indexI--;
  }
  out << output;
  return out;
}

// default constructor from string
LargeNum::LargeNum(const string &str) {
  if (str[0] == '-') {
    isNeg = true;
  }
  for (int i = str.length() - 1; i >= 0; i--) {
    if (isdigit(str[i]))
      myNum.push_back(str[i]);
  }
}

// constructor takes in a int and convert it into string in reverse order
LargeNum::LargeNum(int anInteger) {
  if (anInteger < 0) {
    isNeg = true;
  }
  if (anInteger == 0) {
    myNum = "0";
    return;
  }

  int temp = abs(anInteger); // pass by reference
  while (temp != 0) {
    myNum += to_string(temp % 10);
    temp /= 10;
  }
  // cout << "myNum in Integer constructor is " << myNum << endl; //test codes
}

// use the default copy constructor
// LargeNum::LargeNum(const LargeNum &other){}

// use the default copy assignment operator
// LargeNum &LargeNum::operator=(const LargeNum &other){}

// use the default destructor
// LargeNum::~LargeNum() = default;

// returns true if the number is zero
bool LargeNum::isZero() const { return myNum == "0" && !isNeg; }

// negate the number, positive becomes negative, negative becomes positive
// Zero is always positive
LargeNum &LargeNum::negate() {
  if (!isZero()) {
    isNeg = !isNeg;
  }
  return *this;
}

// add two numbers
LargeNum LargeNum::operator+(const LargeNum &rhs) const {
  if (isNeg == rhs.isNeg) {
    string str = "";
    int len = min((int)myNum.length(),
                  (int)rhs.myNum.length()); // length for the string
    int sum = 0;
    int one = 0;
    int index = 0;

    for (; index < len; index++) {
      sum = (myNum[index] - '0') + (rhs.myNum[index] - '0') + one;
      (sum > 9) ? one = 1 : one = 0;
      str.insert(0, to_string(sum % 10));
    }

    while (index < myNum.size()) {
      sum = (myNum[index++] - '0') + one;
      (sum > 9) ? one = 1 : one = 0;
      str.insert(0, to_string(sum % 10));
    }

    while (index < rhs.myNum.size()) {
      sum = (rhs.myNum[index++] - '0') + one;
      (sum > 9) ? one = 1 : one = 0;
      str.insert(0, to_string(sum % 10));
    }

    if (one == 1)
      str.insert(0, to_string(1));
    if (isNeg && rhs.isNeg)
      str.insert(0, "-");
    // cout << "str is " << str << endl << endl; //test code
    LargeNum retMe(str);
    return retMe;
  }

  if (!isNeg && rhs.isNeg) {
    LargeNum temp(rhs);
    temp.negate();
    return *this - temp;
  }
  if (isNeg && !rhs.isNeg) {
    LargeNum temp(*this);
    temp.negate();
    return rhs - temp;
  }
  return *this;
}

// subtract two numbers
LargeNum LargeNum::operator-(const LargeNum &rhs) const {
  if (isNeg && rhs.isNeg) { // both negative
    LargeNum temp(rhs);
    temp.negate();
    return temp + *this;
  }

  // one of them are negative
  if ((isNeg && !rhs.isNeg) || (!isNeg && rhs.isNeg)) {
    LargeNum temp(rhs);
    temp.negate();
    return *this + temp;
  }

  // both positive
  if (*this < rhs) { // num1 = 90 num2 =100
    LargeNum temp(rhs - *this);
    temp.negate();
    return temp;
  }

  string str = "";
  int len = min((int)myNum.length(),
                (int)rhs.myNum.length()); // length for the string
  int one = 0;
  int ten = 0;
  int sum = 0;
  int indexI = 0;

  for (; indexI < len; indexI++) {
    int num1 = (myNum[indexI] - '0');
    int num2 = (rhs.myNum[indexI] - '0');
    (num1 - one < num2) ? ten = 10 : ten = 0;
    sum = num1 - num2 + ten - one;
    (ten == 10) ? one = 1 : one = 0;
    str.insert(0, to_string(sum));
  }
  while (indexI < myNum.length()) {
    int num1 = (myNum[indexI] - '0');
    (num1 - one < 0) ? ten = 10 : ten = 0;
    sum = num1 + ten - one;
    (ten == 10) ? one = 1 : one = 0;
    str.insert(0, to_string(sum));
    indexI++;
  }

  while (indexI < rhs.myNum.length()) {
    int num1 = (rhs.myNum[indexI] - '0');
    (num1 - one < 0) ? ten = 10 : ten = 0;
    sum = num1 + ten - one;
    (ten == 10) ? one = 1 : one = 0;
    str.insert(0, to_string(sum));
    indexI++;
  }
  // cout << str << endl;
  return LargeNum(stoi(str));
}

// multiply two numbers
LargeNum LargeNum::operator*(const LargeNum &rhs) const {
  if (myNum == "0" || rhs.myNum == "0") {
    return LargeNum(0);
  }

  LargeNum sum(0);
  if (isNeg != rhs.isNeg) {
    sum.isNeg = true;
  }

  vector<vector<string>> retMe;
  int ten = 0;
  int product = 0;
  for (int indexI = 0; indexI < rhs.myNum.length(); indexI++) {
    vector<string> temp{};
    int zero = 0;
    for (; zero < indexI; zero++) {
      temp.push_back("0");
    }
    for (int j = 0; j < myNum.length(); j++) {
      int num1 = (rhs.myNum[indexI] - '0');
      int num2 = (myNum[j] - '0');
      product = ten + (num1 * num2);
      (product >= 10) ? ten = product / 10 : ten = 0;
      temp.push_back(to_string(product % 10));
    }
    if (ten > 0) {
      temp.push_back(to_string(ten));
      ten = 0;
    }
    retMe.push_back(temp);
  }

  //-----------------last---------------------

  for (int i = 0; i < retMe.size(); i++) {
    string std = "";
    for (int j = (int)retMe[i].size() - 1; j >= 0; j--) {
      std.append(retMe[i][j]);
    }
    sum = sum + LargeNum(std);
  }

  return sum; // default value, shouldn't reach here in any situation
}

// return true if the numbers are equal
bool LargeNum::operator==(const LargeNum &rhs) const {
  return myNum == rhs.myNum;
}

// return true if the numbers are not equal
bool LargeNum::operator!=(const LargeNum &rhs) const {
  return myNum != rhs.myNum;
}

// return true if the left-hand-side number is less than the
// right-hand-side number
bool LargeNum::operator<(const LargeNum &rhs) const {
  if (myNum == rhs.myNum) {
    return false;
  }
  if (isNeg != rhs.isNeg) {
    return isNeg; // if one of them is neg
  }
  if (!isNeg) {
    if (myNum.size() != rhs.myNum.size())
      return myNum.size() < rhs.myNum.size();

    // same size
    for (int i = (int)myNum.size() - 1; i >= 0; i--) {
      if ((myNum[i] - '0') != (rhs.myNum[i] - '0'))
        return (myNum[i] - '0') < (rhs.myNum[i] - '0');
    }
  } else {
    if (myNum.size() != rhs.myNum.size())
      return myNum.size() > rhs.myNum.size();

    // same size
    for (int i = (int)myNum.size() - 1; i >= 0; i--) {
      if ((myNum[i] - '0') != (rhs.myNum[i] - '0'))
        return (myNum[i] - '0') > (rhs.myNum[i] - '0');
    }
  }
  return false;
}

// return true if the left-hand-side number is less than or equal to the
// right-hand-side number
bool LargeNum::operator>(const LargeNum &rhs) const {
  if (*this == rhs) {
    return false;
  }
  return !(*this < rhs);
}

// return true if the left-hand-side number is less than the right-hand-side
// number
bool LargeNum::operator<=(const LargeNum &rhs) const {
  if (*this == rhs) {
    return true;
  }
  return (*this < rhs);
}

// return true if the left-hand-side number is greater than or equal to the
// right-hand-side number
bool LargeNum::operator>=(const LargeNum &rhs) const {
  if (*this == rhs) {
    return true;
  }
  return (*this > rhs);
}

// divide two numbers. rhs is the divisor
// similar to integer division, ignore remainder
LargeNum LargeNum::operator/(const LargeNum &rhs) const {
  if (isZero() || rhs.isZero()) {
    return LargeNum(0);
  }

  // get absolute num
  LargeNum absThis(*this);
  LargeNum absRhs(rhs);
  if (absThis.isNeg) {
    absThis.negate();
  }
  if (absRhs.isNeg) {
    absRhs.negate();
  }

  if (absThis < absRhs) {
    return LargeNum(0);
  }
  /*
   25 / 5
   -25 / 5
   25 / -5
   5/25
  */

  LargeNum productFront(absRhs);
  LargeNum productBack(0);
  int pow = 0;
  while (productFront < absThis) {
    pow++;
    LargeNum tempFront(pow);
    LargeNum tempBack(pow + 1); // check the next one
    productFront = absRhs * tempFront;
    productBack = absRhs * tempBack;
    if (productBack > absThis) {
      break;
    }
  }
  LargeNum retMe(pow);
  if (isNeg != rhs.isNeg) {
    retMe.negate();
  }

  // cout << "retMe is : " << retMe << endl;
  return retMe;
}

// postfix increment
LargeNum LargeNum::operator++(int) {
  LargeNum retMe(*this);
  *this = *this + LargeNum(1);
  return retMe;
}

// prefix decrement
LargeNum &LargeNum::operator--() {
  *this = *this - LargeNum(1);
  return *this;
}

// postfix decrement
LargeNum LargeNum::operator--(int) {
  LargeNum retMe(*this);
  *this = *this - LargeNum(1);
  return retMe;
}

// prefix increment
LargeNum &LargeNum::operator++() {
  *this = *this + LargeNum(1);
  return *this;
}
