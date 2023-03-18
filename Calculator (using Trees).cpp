// construction of an expression tree (for binary operators) using RECURSION!

/*
    → find iterative method of forming expression tree
    → add support for unary operators
*/

#include <iostream>
#include <cmath>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

// function to return precedence for binary operators
int getPrecedence(string &s, int j) {
    unordered_map<char, int> pMap;

    pMap['^'] = 3;
    pMap['*'] = 2;
    pMap['/'] = 2;
    pMap['-'] = 1;
    pMap['+'] = 0;

    return pMap[s[j]];
}

class Node {
public:
    int data; // to store operands
    char symbol; // to store operators
    Node* left, *right; // pointer to left and right children

    // constructor for operator
    explicit Node(char c)
        : symbol{c}, left{nullptr}, right{nullptr} {}

    // constructor for operand
    explicit Node(int d)
        : data{d}, symbol{'\0'}, left{nullptr}, right{nullptr} {}
};

class ExpTree {
public:
    Node* root;

    // recursive function to construct expression tree from infix expression
    Node* construct(string s) {
        int minPriority{1000}; // value of minimum priority
        int minj{1000}; // index with maximum priority operator

        int priority{};

        if (s[0] == '-')
            s.insert(s.begin(), {'0'}); // for expression beginning with - sign

        s.insert(s.begin(), {'\0'});

        for (int i{1}, j{1}; j < s.size(); j++) { // loop to find operator with max priority

            // update priority on basis of parenthesis
            if (s[j] == '(') {
                priority += 100;
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

            if (isdigit(s[j]) == false) { // j is the index of operator
                if (minPriority > getPrecedence(s, j) + priority) {
                    minPriority = getPrecedence(s, j) + priority;
                    minj = j;
                }
            }
        }

        // no operator present
        if (minj == 1000) {
            s.erase(s.begin());
            return new Node(stoi(s));
        }

        // construct tree about max priority operator
        Node* ptr = new Node(s[minj]);
        ptr -> left = construct(s.substr(1, minj - 1)); // call construct on left substring
        ptr -> right = construct(s.substr(minj + 1, s.size() - minj - 1)); // call construct on right substring

        return ptr;
    }

    // function for inorder traversal of node
    void printInorder(Node* focusNode) {
        if (focusNode == nullptr)
            return;

        printInorder(focusNode -> left);

        if (focusNode -> symbol == '\0')
            cout << focusNode -> data << "   ";
        else 
            cout << focusNode -> symbol << "   ";

        printInorder(focusNode -> right);
    }

    // ************************ functions to display tree in 2D fromat *************************************
    void space(int d) {
        for (int i{}; i < d; i++)
            cout << " ";
    }
   
    // function to get height of expression tree
    int height(Node* focusNode) {
        if (focusNode == nullptr)
            return 0;

        // find height of subtrees
        int lheight = height(focusNode -> left);
        int rheight = height(focusNode -> right);

        if (lheight > rheight)
            return lheight + 1;
        else
            return rheight + 1;
    }

    // function to print nodes at current level
    void printCurrentLevel(Node* focusNode, int level, int d) {
        if (focusNode -> left == nullptr) {
            focusNode -> left = new Node(' ');
        }

        if (focusNode -> right == nullptr) {
            focusNode -> right = new Node(' ');
        }
        
        if (level == 1) {
            if (focusNode -> symbol == '\0') {
                space(d);
                cout << focusNode -> data;
                space(d);
            }
            else {
                space(d);
                cout << focusNode -> symbol;
                space(d);
            }
        }

        else if (level > 1) {
            printCurrentLevel(focusNode -> left, level - 1, d);
            printCurrentLevel(focusNode -> right, level - 1, d);
        }
    }

    // function for level order traversal
    void printLevelOrder() {
        int h = height(root);
        int w = 2 * pow(2, h);
        for (int i{1}; i <= h; i++) {
            printCurrentLevel(root, i, (w - pow(2, i - 1)) / pow(2, i));
            cout << "\n\n";
        }
    }
    // ***************************************************************************************************
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
    string s{"6 + 9 * 8 / 12 - 45 * 9 + 10 ^ 5"}; // input string

    removeSpace(s);

    ExpTree exp; // creating a variable of type ExpTree

    exp.root = exp.construct(s); // constructing expression tree from input string

    exp.printInorder(exp.root); // inorder traversal
    cout << "\n\n";

    exp.printLevelOrder(); // 2D display
}
