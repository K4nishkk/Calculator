#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include "json.h"
using namespace std;

class Node{
public:
    int data;
    vector<Node*> child;

    explicit Node(int d, int n)
        : data{d} {
            for (int i{}; i < n; i++)
                child.push_back(0);
        }
};

class ExpTree{
public:
    Node* root;

    explicit ExpTree()
        : root{nullptr} {}

    void printLevelOrder();
    void editJsonFile();
    int getHeight(Node* focusNode);
    void getDivSize(vector<vector<pair<double, bool>>> &vec, vector<vector<int>> &arr, Node* focusNode, int i, int k);
};

void ExpTree::printLevelOrder() {
    if (root == nullptr)
        return;

    queue<Node*> q;
    q.push(root);
    int k = 1;

    while (q.empty() == false) {
        Node* temp = q.front();
        cout << temp -> data << "   ";
        q.pop();
        
        for (int i{}; i < temp -> child.size(); i++) {
            q.push(temp -> child[i]);
        }
        
        if (!(--k)) {
            cout << endl;
            k = q.size();
        }
    }
}

void ExpTree::editJsonFile() {
    if (root == nullptr)
        return;
        
    Json::Value event;
    Json::Value JsonVec(Json::arrayValue);

    vector<vector<pair<double, bool>>> vec(getHeight(root));
    vector<vector<int>> arr(getHeight(root));
    getDivSize(vec, arr, root, 0, 1);

    for (int i{}; i < vec.size(); i++) {
        int j{};
        while (j < vec[i].size() - 1) {
            if (vec[i][j].second == 0 && vec[i][j + 1].second == 0) {
                vec[i][j].first = vec[i][j].first + vec[i][j + 1].first;
                vec[i].erase(vec[i].begin() + j + 1);
            }
            else {
                ++j;
            }
        }
    }

    for (int i{}; i < arr.size(); i++) {
        for (int j{}; j < arr[i].size(); j++) {
            cout << arr[i][j] << "   ";
        }
        cout << endl;
    }

    for (int i{}; i < vec.size(); i++) {
        int k{};
        for (int j{}; j < vec[i].size(); j++) {
            JsonVec[i][j]["width"] = vec[i][j].first;
            JsonVec[i][j]["contain"] = vec[i][j].second;
            if (vec[i][j].second == true) {
                JsonVec[i][j]["data"] = arr[i][k++];
            }
        }
    }

    event = JsonVec;

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indention"] = "\t";

    unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    ofstream outputFileStream("details.json");
    writer -> write(event, &outputFileStream);
}

int ExpTree::getHeight(Node* focusNode) {
    if (root == nullptr)
        return 0;

    if (focusNode -> child.size() == 0)
        return 1;

    int max = 1;
    for (int i{}; i < focusNode -> child.size(); i++) {
        int h = getHeight(focusNode -> child[i]);
        if (h > max)
            max = h;
    }

    return max + 1;
}

void ExpTree::getDivSize(vector<vector<pair<double, bool>>> &vec, vector<vector<int>> &arr, Node* focusNode, int i = 0, int k = 1) {
    vec[i].push_back({(double)100 / k, 1});
    arr[i].push_back(focusNode -> data);

    if (focusNode -> child.size() == 0) {
        for (int j{i + 1}; j < vec.size(); j++) {
            vec[j].push_back({(double)100 / k, 0});
        }
    }

    else {
        for (int j{}; j < focusNode -> child.size(); j++) {
            getDivSize(vec, arr, focusNode -> child[j], i + 1, k * (focusNode -> child.size()));
        }
    }
}

int main() {
    ExpTree tree;
    tree.root = new Node(1, 2);
    tree.root -> child[0] = new Node(2, 3);
    tree.root -> child[0] -> child[0] = new Node(3, 0);
    tree.root -> child[0] -> child[1] = new Node(4, 3);
    tree.root -> child[0] -> child[1]  -> child[0] = new Node(16, 0);
    tree.root -> child[0] -> child[1]  -> child[1]  = new Node(17, 0);
    tree.root -> child[0] -> child[1]  -> child[2]  = new Node(18, 0);
    tree.root -> child[0] -> child[2] = new Node(5, 0);
    tree.root -> child[1] = new Node(6, 6);
    tree.root -> child[1] -> child[0] = new Node(7, 0);
    tree.root -> child[1] -> child[1] = new Node(8, 0);
    tree.root -> child[1] -> child[2] = new Node(9, 0);
    tree.root -> child[1] -> child[3] = new Node(10, 1);
    tree.root -> child[1] -> child[3] -> child[0] = new Node(13, 2);
    tree.root -> child[1] -> child[3] -> child[0] -> child[0] = new Node(14, 0);
    tree.root -> child[1] -> child[3] -> child[0] -> child[1] = new Node(15, 0);
    tree.root -> child[1] -> child[4] = new Node(11, 0);
    tree.root -> child[1] -> child[5] = new Node(12, 0);

    tree.printLevelOrder();
    tree.editJsonFile();
}
