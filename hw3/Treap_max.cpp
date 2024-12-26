// Treap.cpp
#include <algorithm> // 加入這一行以使用 max_element
#include <chrono>
#include <iostream>
#include <queue>
#include <vector>
#include <math.h>

using namespace std;
using namespace std::chrono;

// 節點結構
struct Node {
    int value;
    int priority; // Treap 的優先權
    Node *left;
    Node *right;

    Node(int val, int pri = rand())
        : value(val), priority(pri), left(nullptr), right(nullptr) {}
};

typedef Node *addr;

// 右旋
addr rotateRight(addr y) {
    addr x = y->left;
    addr T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

// 左旋
addr rotateLeft(addr x) {
    addr y = x->right;
    addr T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

// 1. CreateTreap
addr CreateTreap(int num) { return new Node(num); }

// 2. InsertTreap
addr InsertTreap(int val, int pri, addr root) {
    if (root == nullptr) {
        return new Node(val, pri);
    }

    if (val < root->value) {
        root->left = InsertTreap(val, pri, root->left);

        if (root->left->priority > root->priority) {
            root = rotateRight(root);
        }
    } else if (val > root->value) {
        root->right = InsertTreap(val, pri, root->right);

        if (root->right->priority > root->priority) {
            root = rotateLeft(root);
        }
    }

    return root;
}

// 3. PrintTreap by Level
void PrintTreap(addr root) {
    if (root == nullptr) {
        cout << "Tree is empty." << endl;
        return;
    }

    cout << "Tree values by level:" << endl;
    queue<addr> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        bool hasNextLevel = false;
        for (int i = 0; i < levelSize; ++i) {
            addr current = q.front();
            q.pop();

            if (current) {
                cout << current->value << "(" << current->priority << ") ";
                q.push(current->left);
                q.push(current->right);

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

        if (!hasNextLevel)
            break;
    }
}

// 4. HeightTreap
int HeightTreap(addr root) {
    if (root == nullptr) {
        return 0;
    }
    return max(HeightTreap(root->left), HeightTreap(root->right)) + 1;
}

void testTreap() {
    addr root = nullptr;

    // 按指定順序插入節點及優先權
    root = InsertTreap(3, 0.9, root);
    root = InsertTreap(2, 0.5, root);
    root = InsertTreap(1, 0.3, root);
    root = InsertTreap(5, 0.2, root);
    root = InsertTreap(4, 0.1, root);

    // 列印 Treap
    PrintTreap(root);

    // 計算 Treap 高度
    int height = HeightTreap(root);
    cout << "Height: " << height << endl;
}

int main(int argc, char *argv[]) {

    testTreap();

    // if (argc < 2) {
    //     cerr << "Usage: " << argv[0] << " <n> <mode>\n";
    //     return 1;
    // }

    // int n = pow(2, 20);
    // int k_value = stoi(argv[1]); // k值

    // const int numExperiments = 10; // 設定重複次數
    // double totalDuration = 0;      // 記錄總時間

    // srand(std::time(nullptr));

    // addr out_root;

    // for (int experiment = 0; experiment < numExperiments; experiment++) {
    //     int num = 1;
    //     std::vector<int> arr(n);
    //     for (int &x : arr)
    //         x = num++;
    //     for (int i = 0; i < k_value; i++) { // k次交換
    //         int random1 = rand() % n;
    //         int random2 = rand() % n;
    //         swap(arr[random1], arr[random2]);
    //     }
    //     auto start = std::chrono::high_resolution_clock::now();
    //     addr root = CreateTreap(arr[0]);
    //     for (int i = 0; i < n; i++) {
    //         root = InsertTreap(arr[i], rand(),root);
    //     }
    //     auto end = std::chrono::high_resolution_clock::now();
    //     duration<double> duration = end - start;
    //     totalDuration += duration.count();
    //     out_root = root;
    // }
    // double avgDuration = totalDuration / numExperiments;
    // cout << k_value << "," << avgDuration << "," << HeightTreap(out_root) << endl;
    return 0;
}
