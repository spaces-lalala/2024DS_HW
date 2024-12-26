// BST.cpp
#include <chrono>
#include <iostream>
#include <math.h>
#include <queue>
#include <random>
#include <vector>

using namespace std;
using namespace std::chrono;

// 節點結構
struct Node {
    int value;
    Node *left;
    Node *right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

typedef Node *addr;
int HeightBST(addr root);

// 1. CreateBST
addr CreateBST(int num) { return new Node(num); }

// 2. InsertBST
addr InsertBST(int num, addr root) {
    if (root == nullptr) {
        return new Node(num);
    }
    if (num < root->value) {
        root->left = InsertBST(num, root->left);
    } else if (num > root->value) {
        root->right = InsertBST(num, root->right);
    }
    // 如果 num 已存在，不做任何操作
    return root;
}

// 3. PrintBST
void PrintBST(addr root) {
    if (root == nullptr) {
        cout << "Tree is empty." << endl;
        return;
    }
    int height = HeightBST(root);
    cout << "Height of the tree: " << height << endl;
    queue<addr> q;
    q.push(root);
    cout << "Tree values by level:" << endl;
    while (!q.empty()) {
        int levelSize = q.size();
        bool hasNextLevel = false; // 用於檢查下一層是否有真實節點
        for (int i = 0; i < levelSize; ++i) {
            addr current = q.front();
            q.pop();
            if (current) {
                cout << current->value << " ";
                q.push(current->left);
                q.push(current->right);

                // 如果子節點中有非空，標記為需要繼續打印
                if (current->left || current->right) {
                    hasNextLevel = true;
                }
            } else {
                cout << "null ";
                q.push(nullptr);
                q.push(nullptr);
            }
        }
        cout << endl;
        // 如果下一層全為空，停止處理
        if (!hasNextLevel)
            break;
    }
}

// 4. HeightBST
int HeightBST(addr root) {
    if (root == nullptr)
        return 0;
    int leftHeight = HeightBST(root->left);
    int rightHeight = HeightBST(root->right);
    return max(leftHeight, rightHeight) + 1;
}

void testBST(void) {
    cout << "Testing BST" << endl;

    addr root = CreateBST(3);
    root = InsertBST(2, root);
    root = InsertBST(1, root);
    root = InsertBST(5, root);
    root = InsertBST(4, root);
    PrintBST(root);
    int height = HeightBST(root);
    cout << "Height: " << height << endl;
}

int main(int argc, char *argv[]) {

    // testBST();
    // cout << "1";

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = pow(2, 20);
    int k_value = stoi(argv[1]); // k值

    const int numExperiments = 10; // 設定重複次數
    double totalDuration = 0;     // 記錄總時間
    double totalheight = 0;

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int experiment = 0; experiment < numExperiments; experiment++) {
        int num = 1;
        std::vector<int> arr(n);
        for (int &x : arr)
            x = num++;
        std::uniform_int_distribution<> dis(0, n - 1);

        for (int i = 0; i < k_value; i++) {
            int random1 = dis(gen);
            int random2 = dis(gen);
            swap(arr[random1], arr[random2]);
        }
        auto start = std::chrono::high_resolution_clock::now();
        addr root = CreateBST(arr[0]);
        for (int i = 0; i < n; i++) {
            root = InsertBST(arr[i], root);
        }
        auto end = std::chrono::high_resolution_clock::now();
        duration<double> duration = end - start;
        totalDuration += duration.count();
        totalheight += HeightBST(root);
    }
    double avgDuration = totalDuration / numExperiments;
    double avgHeight = totalheight / numExperiments;

    cout << k_value << "," << avgDuration << "," << avgHeight << endl;
    return 0;
}
