// construction of an expression tree (for binary operators)

/*
    → add parenthesis priority
    → add support for unary operators
*/

// Aim - Construct expression tree from string expression
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Node{
public:
    int data;
    char symbol;
    Node* left, *right;

    // constructor
    explicit Node(int d) 
        : data{d}, symbol{'\0'}, left{nullptr}, right{nullptr} {}

    explicit Node(char c)
        : symbol{c}, left{nullptr}, right{nullptr} {}
};

class ExpTree{
public:
    Node* root;

    // constructor
    ExpTree()
        : root{nullptr} {}

    Node* insertNode(char c);
    void insertNode(int d, Node* ptr);
    int getPrecedence(char c);
    void split(string s);
    void printInorder(Node* focusNode);

    int getHeight(Node* focusNode);
    void printLevelOrder();
    void printCurrentLevel(Node* focusNode, int level);
};

// return pointer to empty node after inserting operand
Node* ExpTree::insertNode(char c) {
    if (root -> symbol == '\0' || getPrecedence(root -> symbol) > getPrecedence(c)) {
        Node* temp = new Node(c);
        temp -> left = root;
        root = temp;
        return root;
    }

    Node* focusNode = root;

    while (getPrecedence(c) > getPrecedence(focusNode -> right -> symbol) && focusNode -> right -> symbol != '\0') {
        focusNode = focusNode -> right;
    }

    Node* temp = new Node(c);
    temp -> left = focusNode -> right;
    focusNode -> right = temp;
    return focusNode -> right;
}

void ExpTree::insertNode(int d, Node* ptr) {
    if (root == nullptr)
        root = new Node(d);
    else
        ptr -> right = new Node(d);
}

// function to return precedence for binary operators
int ExpTree::getPrecedence(char c) {
    unordered_map<char, int> pMap;

    pMap['^'] = 3;
    pMap['*'] = 2;
    pMap['/'] = 2;
    pMap['-'] = 1;
    pMap['+'] = 0;

    return pMap[c];
}

void ExpTree::split(string s) {

    int priority{0};
    Node* temp{nullptr};

    if (s[0] == '-')
        s.insert(s.begin(), {'0'}); // for expression beginning with - sign

    s.insert(s.begin(), '\0');

    for (int i{1}, j{1}; j < s.size(); j++) {

        // update priority on basis of paranthesis
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

        if (isdigit(s[j]) == false) // j is an operator
            temp = insertNode(s[j]);

        else {
            if (isdigit(s[j]) == true && isdigit(s[j - 1]) == false) // i is the start of integer
                i = j;
            if (isdigit(s[j]) == true && isdigit(s[j + 1]) == false) // j is end of integer
                insertNode(stoi(s.substr(i, j - i + 1)), temp);            
        }
    }
}

void ExpTree::printInorder(Node* focusNode) {
    if (focusNode == nullptr)
        return;

    printInorder(focusNode -> left);
    if (focusNode -> symbol == '\0')
        cout << focusNode -> data << "   ";
    else 
        cout << focusNode -> symbol << "   ";
    printInorder(focusNode -> right);
}

int ExpTree::getHeight(Node* focusNode) {
    if (focusNode == nullptr)
        return 0;

    int lheight = getHeight(focusNode -> left);
    int rheight = getHeight(focusNode -> right);

    if (lheight > rheight)
        return lheight + 1;
    else 
        return rheight + 1;
}

void ExpTree::printLevelOrder() {
    int h = getHeight(root);

    for (int i{}; i < h; i++) {
        printCurrentLevel(root, i);
        cout << endl;
    }
}

void ExpTree::printCurrentLevel(Node* focusNode, int level) {
    if (focusNode == nullptr)
        return;

    else if (level == 0) {
        if (focusNode -> symbol == '\0')
            cout << focusNode -> data << "   ";
        else 
            cout << focusNode -> symbol << "   ";
        return;
    }

    printCurrentLevel(focusNode -> left, level - 1);
    printCurrentLevel(focusNode -> right, level - 1);
}

int main() {
    string s{"-12+36-89*45/14"};
    ExpTree exptree;
    exptree.split(s);
    exptree.printInorder(exptree.root);
    cout << endl;
    exptree.printLevelOrder();
}
