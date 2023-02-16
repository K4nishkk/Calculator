/*

Simple calculator program using vector
    → removeSpace() for removing white spaces
    → split() to split input string into vector seperating operators and operands
    → printVec() to display vector
    → cal() to calculate result

pros:
    → for combination of integer operands and +, -, *, / operations
    → simple

cons:
    → cannot start with negative integer
    → not for double or floating operands
    → not precise (due to / operation and integer result)
    → high time complexity (multiple while loops)
    → try to use stack or expression tree, etc.

*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// function to remove white spaces
void removeSpace(string &str) {
    for (int i{}; i < str.size(); i++) {
        if (str[i] == ' ') {
            str.erase(str.begin() + i);
            i--;
        }
    }
}

// function to print the vector
void printVec(vector<string> &vec) {
    for (string s : vec)
        cout << s << "   ";
}

// function to split integers and operators and return it as a vector
vector<string> split(string s) {
    vector<string> vec{}; // storage vector

    if (s[0] == '-')
        s.insert(s.begin(), {'0'}); // to have negative int at beginning

    for (int i{}, j{}; j < s.size() || i < s.size(); j++) {
        if (isdigit(s[j]) == false) {
            vec.insert(vec.end(), {s.substr(i, j - i), s.substr(j, 1)}); // inserting operand and operator at the end
            i = j + 1; // assigning first digit posititon to leading pointer
        }
    }

    vec.pop_back(); // deleting last null character
    return vec;
}

int cal(vector<string> &vec) {
    vector<string>::iterator i;

    // for * operator
    while (find(vec.begin(), vec.end(), "*") != vec.end()) {
        i = find(vec.begin(), vec.end(), "*");
        int operand1 = stoi(vec[i - vec.begin() - 1]);
        int operand2 = stoi(vec[i - vec.begin() + 1]);
        int result = operand1 * operand2;
        vec.erase(i + 1);
        vec.erase(i);
        vec.at(i - 1 - vec.begin()) = to_string(result);
    }

    // for / operator
    while (find(vec.begin(), vec.end(), "/") != vec.end()) {
        i = find(vec.begin(), vec.end(), "/");
        int operand1 = stoi(vec[i - vec.begin() - 1]);
        int operand2 = stoi(vec[i - vec.begin() + 1]);
        int result = operand1 / operand2;
        vec.erase(i + 1);
        vec.erase(i);
        vec.at(i - 1 - vec.begin()) = to_string(result);
    }

    // for - operator
    // "-" needs to be before "+"
    // if not: a - b + c = a - (b + c) happens
    while (find(vec.begin(), vec.end(), "-") != vec.end()) {
        i = find(vec.begin(), vec.end(), "-");
        int operand1 = stoi(vec[i - vec.begin() - 1]);
        int operand2 = stoi(vec[i - vec.begin() + 1]);
        int result = operand1 - operand2;
        vec.erase(i + 1);
        vec.erase(i);
        vec.at(i - 1 - vec.begin()) = to_string(result);
    }
    
    // for + operator
    while (find(vec.begin(), vec.end(), "+") != vec.end()) {
        i = find(vec.begin(), vec.end(), "+");
        int operand1 = stoi(vec[i - vec.begin() - 1]);
        int operand2 = stoi(vec[i - vec.begin() + 1]);
        int result = operand1 + operand2;
        vec.erase(i + 1);
        vec.erase(i);
        vec.at(i - 1 - vec.begin()) = to_string(result);
    }

    return stoi(vec[0]);
}

int main () {
    string s{"-54+69 *  58+111 - 255/ 12 +9"}; // input expression
    
    // string s{}; // for user input
    // cout << "Input expression: ";
    // getline(cin, s);

    removeSpace(s); // to deal with white spaces

    vector<string> vec = split(s); // split into vector

    printVec(vec);

    int result = cal(vec); // calculate result
    cout << "=   " << result << endl;
}
