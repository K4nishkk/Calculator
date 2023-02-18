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
    → works for two operator situation
    → simple

cons:
    → nested paranthesis not allowed
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
int getPrecedence1(char c, int priority, int& count) {
    unordered_map<char, int> pMap;
    pMap['^'] = 3;
    pMap['*'] = 2;
    pMap['/'] = 2;
    pMap['-'] = 1;
    pMap['+'] = 0;

    if (count != 0) {
        --count;
        return pMap[c] + priority - 100;
    }
    else 
        return pMap[c] + priority;
}

// function to return precedence for unary operators
int getPrecedence0(char c, int priority, int& count) {
    unordered_map<char, int> pMap;
    pMap['-'] = 11;
    pMap['+'] = 10;

    if (count != 0) {
        --count;
        return pMap[c] + priority - 100;
    }
    else 
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

        int priority{0};
        int count{0};

        if (s[0] == '-')
            s.insert(s.begin(), {'0'}); // for expression beginning with - sign

        for (int i{}, j{}; j < s.size(); j++) {

            // update priority on basis of paranthesis
            if (s[j] == '(') {
                priority += 100;
                for (int k{j - 1}; isdigit(s[k]) == false; --k) // count number of operators outside paranthesis
                    ++count;
                s.erase(s.begin() + j);
            }
            if (s[j] == ')') {
                priority -= 100;
                s.erase(s.begin() + j);
            }

            // parsing
            if (i == 0 && isdigit(s[j]) == true && isdigit(s[j + 1]) == false) // for integer at beginning of expression
                insertNode(stoi(s.substr(i, j - i + 1)));
            
            else {
                if (isdigit(s[j]) == true && isdigit(s[j - 1]) == false) // i is the start of integer
                    i = j;
                
                if (isdigit(s[j]) == true && isdigit(s[j + 1]) == false) { // j is end of integer
                        
                    if (isdigit(s[i - 2]) == false) {
                        pMap.insert(pair<int, Node*>(getPrecedence1(s[i - 2], priority, count), insertNode(s[i - 2])));
                        pMap.insert(pair<int, Node*>(getPrecedence0(s[i - 1], priority, count), insertNode(s[i - 1])));
                        insertNode(stoi(s.substr(i, j - i + 1)));
                    }
                    else {
                        pMap.insert(pair<int, Node*>(getPrecedence1(s[i - 1], priority, count), insertNode(s[i - 1])));
                        insertNode(stoi(s.substr(i, j - i + 1)));
                    }
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
            
            // replace (operand1-symbol-operand2) with (result) node
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

            else if (n -> prev -> symbol == '\0') // for binary operators
                cout << n -> symbol << "   ";

            else { // for unary operators (problem with precedence)
                cout << "(" << n -> symbol;
                n = n -> next;
                cout << n -> data << ")   ";
            }
            
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
    string s{"-100*-12+-4/(-6-19)-66*+7^(5-2)"}; // input expression
    
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
    cout << "=   " << result << endl; // display result
}
