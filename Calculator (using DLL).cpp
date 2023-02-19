/*

Simple calculator program using doubly linked list
    → Node class
    → DLL class
    → getPrecedence0() to get precedence of unary operator
    → getPrecedence1() to get precedence of binary operator
    → removeSpace() for removing white spaces
    → overloaded insertNode() to insert operand and operator
    → split() to split input string into DLL nodes seperating operators and operands
    → printList() to display vector
    → cal() to calculate result
    → evaluate() to evalute expression about certain operator and update DLL

pros:
    → for combination of integer operands and +, -, *, /, ^, () operations
    → nested paranthesis are allowed
    → mulitple simultaneous operators allowed

cons:
    → not for double or floating operands
    → not precise (due to / operation and integer result)
    → try to use stack or expression tree, etc.

*/
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <map>
#include <iterator>
using namespace std;

// function to return precedence for binary operators
int getPrecedence1(char c, int priority) {
    unordered_map<char, int> pMap;
    pMap['^'] = 3;
    pMap['*'] = 2;
    pMap['/'] = 2;
    pMap['-'] = 1;
    pMap['+'] = 0;

    return pMap[c] + priority;
}

// function to return precedence for unary operators
int getPrecedence0(char c, int priority) {
    unordered_map<char, int> pMap;
    pMap['-'] = 11;
    pMap['+'] = 10;
    
    return pMap[c] + priority;
}

class Node{
public:
    int data; // to store integer
    char symbol; // mathematical operation symbol
    Node* prev, *next; // pointers to previous and next Node

    // constructor
    explicit Node(int d) 
        : data{d}, symbol{'\0'}, prev{nullptr}, next{nullptr} {}

    explicit Node(char c)
        : symbol{c}, prev{nullptr}, next{nullptr} {}
};

class DLL{
public:
    Node* head;

    // constructor
    DLL()
        : head{nullptr} {}

    // function to insert node
    void insertNode(int d) {
        if (head == nullptr) {
            head = new Node(d);
            return;
        }

        Node* n{head};
        while (n -> next != nullptr) 
            n = n -> next;
        Node* ptr = new Node(d);
        n -> next = ptr;
        ptr -> prev = n;
    }

    Node* insertNode(char c) {
        if (head == nullptr) {
            head = new Node(c);
            return head;
        }

        Node* n{head};
        while (n -> next != nullptr) 
            n = n -> next;
        Node* ptr = new Node(c);
        n -> next = ptr;
        ptr -> prev = n;

        return ptr;
    }

    // function to split expression into nodes and 
    // return a multimap <priority, corresponding pointer to operator node>
    multimap<int, Node*> split(string s) {

        multimap<int, Node*> pMap;

        unordered_map<int, int> mFactor;

        int priority{0};

        if (s[0] == '-')
            s.insert(s.begin(), {'0'}); // for expression beginning with - sign

        for (int i{}, j{}; j < s.size(); j++) {

            // update priority on basis of paranthesis
            if (s[j] == '(') {
                priority += 100;
                for (int k{j - 1}; isdigit(s[k]) == false; --k) {// count number of operators outside paranthesis
                    mFactor[k] += 100;
                }
                s.erase(s.begin() + j);
                j--;
                continue;
            }
            if (s[j] == ')') {
                priority -= 100;
                s.erase(s.begin() + j);
                j--;
                continue;
            }

            // parsing
            if (i == 0 && isdigit(s[j]) == true && isdigit(s[j + 1]) == false) {// for integer at beginning of expression
                insertNode(stoi(s.substr(i, j - i + 1)));
                i = j + 1;
            }
            
            else {
                if (isdigit(s[j]) == true && isdigit(s[j - 1]) == false) // i is the start of integer
                    i = j;
                
                if (isdigit(s[j]) == true && isdigit(s[j + 1]) == false) { // j is end of integer

                    int k{i - 1};

                    while (isdigit(s[k]) == false) // count number of operators before integer
                        --k;

                    for (k = k + 1; k < i; k++) {
                        if (isdigit(s[k - 1]) == true) // append binary operator
                            pMap.insert(pair<int, Node*>(getPrecedence1(s[k], priority - mFactor[k]), insertNode(s[k])));
                        else // append unary operator
                            pMap.insert(pair<int, Node*>(getPrecedence0(s[k], priority - mFactor[k]), insertNode(s[k])));
                    }
                    insertNode(stoi(s.substr(i, j - i + 1)));
                }
            }
        }

        return pMap;
    }

    void evaluate(Node* n) {
        // FOR BINARY OPERATOR
        if (n -> prev -> symbol == '\0') { // previous nodes contains a digit which means binary operator
            int operand1 = n -> prev -> data;
            int operand2 = n -> next -> data;
            int result;

            switch (n -> symbol) {
                case '+':
                    result = operand1 + operand2;
                    break;

                case '-':
                    result = operand1 - operand2;
                    break;

                case '*':
                    result = operand1 * operand2;
                    break;

                case '/':
                    result = operand1 / operand2;
                    break;

                case '^':
                    result = pow(operand1, operand2);
                    break;
                
                default:
                    break;
            }

            Node* ptr = new Node(result);
            if (n -> prev -> prev == nullptr)
                head = ptr;
            else 
                n -> prev -> prev -> next = ptr;

            if (n -> next -> next != nullptr)
                n -> next -> next -> prev = ptr;

            ptr -> prev = n -> prev -> prev;
            ptr -> next = n -> next -> next;
        }

        // FOR UNARY OPERATOR
        else {
            int operand2 = n -> next -> data;
            int result;  

            switch (n -> symbol) { // evalute result
                case '+':
                    result = 0 + operand2;
                    break;

                case '-':
                    result = 0 - operand2;
                    break;
                
                default:
                    break;
            }

            // replace (symbol-operator2) with (result) node
            Node* ptr = new Node(result); 
            if (n -> prev == nullptr)
                head = ptr;
            else 
                n -> prev -> next = ptr;

            if (n -> next -> next != nullptr)
                n -> next -> next -> prev = ptr;

            ptr -> prev = n -> prev;
            ptr -> next = n -> next -> next;

        }
    }

    // function to calculate expression 
    int cal(multimap<int, Node*> pMap) {

        multimap<int, Node*>::reverse_iterator i;

        // priority stored in increasing order
        // backward looping so that higher priority gets solve firsts
        for (i = pMap.rbegin(); i != pMap.rend(); i++)
            evaluate(i -> second);

        return head -> data;
    }

    // function to print list
    void printList() {
        if (head == nullptr)
            return;

        Node* n{head};
        while (n != nullptr) {
            if (n -> symbol == '\0') // for operands
                cout << n -> data << "   ";

            else // for operators
                cout << n -> symbol << "   ";
            
            n = n -> next;
        }
    }
};

// function to remove white spaces
void removeSpace(string &str) {
    for (int i{}; i < str.size(); i++) {
        if (str[i] == ' ') {
            str.erase(str.begin() + i);
            i--;
        }
    }
}

int main () {
    string s{"-55*7+(-12+34/-(4*(47++-6))/-19)---4^5"}; // input expression
    
    // string s{}; // for user input
    // cout << "Input expression: ";
    // getline(cin, s);

    removeSpace(s); // to deal with white spaces

    DLL exp; // creating doubly linked list

    // unordered_map: multiple keys can have same value
    // multimap: multiple values can have same keys

    multimap<int, Node*> pMap = exp.split(s); // split expression in DLL and store pointer to operator in order of precedence

    // multimap<int, Node*>::iterator i; // print operator priority table
    // for (i = pMap.begin(); i != pMap.end(); i++)
    //     cout << "\t" << i -> first << "\t" << (i -> second) -> symbol << endl;

    exp.printList();

    int result = exp.cal(pMap); // calculate result
    cout << "=   " << result << endl;
}
