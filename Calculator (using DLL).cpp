/*

Simple calculator program using doubly linked list
    → Node class
    → DLL class
    → getPrecedence() to return operator precedence
    → removeSpace() for removing white spaces
    → overloaded insertNode() to insert operand and operator
    → split() to split input string into DLL nodes seperating operators and operands
    → printList() to display vector
    → cal() to calculate result
    → evaluate() to evalute expression about certain operator and update DLL

pros:
    → for combination of integer operands and +, -, *, /, ^, () operations
    → simple

cons:
    → doesn't work if - sign outside parenthesis
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

// function to return precedence
int getPrecedence(char c) {
    unordered_map<char, int> pMap;
    pMap['^'] = 3;
    pMap['*'] = 2;
    pMap['/'] = 2;
    pMap['-'] = 1;
    pMap['+'] = 0;

    return pMap[c];
}

class Node{
public:
    int data; // to store integer
    char symbol; // mathematical operation symbol
    Node* prev, *next; // pointers to previous and next Node

    // constructor
    explicit Node(int d) 
        : data{d}, prev{nullptr}, next{nullptr} {}

    explicit Node(char c)
        : symbol{c}, prev{nullptr}, next{nullptr} {}
};

class DLL{
public:
    Node* head;

    // constructor
    DLL()
        : head{nullptr} {}

    // function to insert operand node
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

    // function to insert operator node and return a pointer to it
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
    // return a multimap <priority, corresponding pointer to operator node> for calculation
    multimap<int, Node*> split(string s) {

        multimap<int, Node*> pMap;

        // priority of operator before '(' got incremented
        // to handle that we need to use flag and p
        // working --- '(' found -> operator outside gets inserted in DLL -> ++priority
        int priority{0}; 
        bool flag = 0;
        int p;

        if (s[0] == '-')
            s.insert(s.begin(), {'0'}); // for expression beginning with - sign

        for (int i{}, j{}; j < s.size(); j++) {

            if (s[j] == '(') {
                priority += 10;
                flag = 1;
                s.erase(s.begin() + j);
            }
            if (s[j] == ')') {
                priority -= 10;
                s.erase(s.begin() + j);
            }

            if (i == 0 && isdigit(s[j]) == true && isdigit(s[j + 1]) == false) { // for integer at beginning of expression
                insertNode(stoi(s.substr(i, j - i + 1)));
                i = j + 1;
            }
            else {
                if (isdigit(s[j]) == true && isdigit(s[j - 1]) == false) // i is the start of integer
                    i = j;
                
                if (isdigit(s[j]) == true && isdigit(s[j + 1]) == false) { // j is end of integer

                    // handling priority due to paranthesis
                    if (flag) {
                        p = priority - 10;
                        flag = 0;
                    }
                    else
                        p = priority;
                        
                    if (isdigit(s[i - 2]) == false) {
                        Node* ptr = insertNode(s[i - 2]);
                        pMap.insert(pair<int, Node*>(p + getPrecedence(s[i - 2]), ptr));
                        insertNode(stoi(s.substr(i - 1, j - i + 2)));
                    }
                    else {
                        Node* ptr = insertNode(s[i - 1]);
                        pMap.insert(pair<int, Node*>(p + getPrecedence(s[i - 1]), ptr));
                        insertNode(stoi(s.substr(i, j - i + 1)));
                    }
                }
            }
        }

        return pMap;
    }

    void evaluate(Node* n) {
        int operand1 = n -> prev -> data;
        int operand2 = n -> next -> data;
        int result;

        switch (n -> symbol) { // evalute result
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

    // function to calculate expression 
    int cal(multimap<int, Node*> pMap) {

        multimap<int, Node*>::reverse_iterator i;

        // priority stored in increasing order
        // backward looping so that higher priority gets solve firsts
        for (i = pMap.rbegin(); i != pMap.rend(); i++) {
            evaluate(i -> second);
        }

        return head -> data;
    }

    // function to print list
    void printList() {
        if (head == nullptr)
            return;

        Node* n{head};
        while (true) {
            cout << n -> data << "   ";
            n = n -> next;

            if (n == nullptr)
                return;

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
    string s{"-54+-69 *  (-58+111) - -255/ +12 +9^2"}; // input expression
    
    // string s{}; // for user input
    // cout << "Input expression: ";
    // getline(cin, s);

    removeSpace(s); // to deal with white spaces

    DLL exp; // creating doubly linked list

    // unordered_map: multiple keys can have same value
    // multimap: multiple values can have same keys

    multimap<int, Node*> pMap = exp.split(s); // split in DLL and assigning pMap with node pointer and priority

    exp.printList(); // print DLL

    int result = exp.cal(pMap); // calculate result
    cout << "=   " << result << endl; // display result
}
