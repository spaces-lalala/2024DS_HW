// AVL.cpp
#include <algorithm> // 加入這一行以使用 max_element
#include <chrono>
#include <iostream>
#include <math.h>
#include <queue>
#include <random>
#include <vector>
#include <list>
#include <iterator>

using namespace std;
using namespace std::chrono;

struct Node {
    int value;
    int height;
    Node *left;
    Node *right;

    Node(int val) : value(val), height(1), left(nullptr), right(nullptr) {}
};

typedef Node *addr;

// 計算節點高度
int getHeight(addr node) { return node == nullptr ? 0 : node->height; }

// 計算平衡因子
int getBalance(addr node) {
    return node == nullptr ? 0 : getHeight(node->left) - getHeight(node->right);
}

// 更新節點高度
void updateHeight(addr node) {
    if (node != nullptr) {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }
}

// 左旋
addr rotateLeft(addr x) {
    addr y = x->right;
    addr T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// 右旋
addr rotateRight(addr y) {
    addr x = y->left;
    addr T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

int HeightAVL(addr root);

// 1. CreateAVL
addr CreateAVL(int num) { return new Node(num); }

// 2. InsertAVL
addr InsertAVL(int num, addr root) {
    if (root == nullptr) {
        return new Node(num);
    }

    if (num < root->value) {
        root->left = InsertAVL(num, root->left);
    } else if (num > root->value) {
        root->right = InsertAVL(num, root->right);
    } else {
        // 重複值，不插入
        return root;
    }

    updateHeight(root);

    int balance = getBalance(root);

    // LL Case
    if (balance > 1 && num < root->left->value) {
        return rotateRight(root);
    }

    // RR Case
    if (balance < -1 && num > root->right->value) {
        return rotateLeft(root);
    }

    // LR Case
    if (balance > 1 && num > root->left->value) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // RL Case
    if (balance < -1 && num < root->right->value) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// 3. PrintAVL
void PrintAVL(addr root) {
    if (root == nullptr) {
        cout << "Tree is empty." << endl;
        return;
    }

    int height = HeightAVL(root);
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

// 4. HeightAVL
int HeightAVL(addr root) { return getHeight(root); }

void testAVL(void) {
    cout << "Testing AVL" << endl;

    addr root = CreateAVL(1);
    PrintAVL(root);
    root = InsertAVL(2, root);
    PrintAVL(root);
    root = InsertAVL(3, root);
    PrintAVL(root);
    root = InsertAVL(4, root);
    PrintAVL(root);
    root = InsertAVL(5, root);
    PrintAVL(root);
    root = InsertAVL(6, root);
    PrintAVL(root);
    root = InsertAVL(7, root);
    PrintAVL(root);
    int height = HeightAVL(root);
    cout << "Height: " << height << endl;
}

int main(int argc, char *argv[]) {

    // testAVL();

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = pow(2, 20);
    int k_value = stoi(argv[1]); // k值

    const int numExperiments = 10; // 設定重複次數
    double totalDuration = 0;      // 記錄總時間
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
        addr root = CreateAVL(arr[0]);
        for (int i = 1; i < n; i++) {
            root = InsertAVL(arr[i], root);
        }
        auto end = std::chrono::high_resolution_clock::now();
        duration<double> duration = end - start;
        totalDuration += duration.count();
        totalheight += HeightAVL(root);
    }
    double avgDuration = totalDuration / numExperiments;
    double avgHeight = totalheight / numExperiments;
    cout << k_value << "," << avgDuration << "," << avgHeight << endl;
    return 0;
}
