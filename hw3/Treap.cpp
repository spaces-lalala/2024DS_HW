// Treap.cpp
#include <algorithm>
#include <chrono>
#include <iostream>
#include <queue>
#include <vector>
#include <random> // 用於 normal_distribution

using namespace std;
using namespace std::chrono;

// 節點結構
struct Node {
    int value;
    float priority; 
    Node *left;
    Node *right;

    Node(int val, float pri) : value(val), priority(pri), left(nullptr), right(nullptr) {}
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

// 1.CreateTreap
addr CreateTreap(int num, float pri) { return new Node(num, pri); }

// 2.InsertTreap
addr InsertTreap(int val, float pri, addr root) {
    if (root == nullptr) {
        return new Node(val, pri);
    }

    if (val < root->value) {
        root->left = InsertTreap(val, pri, root->left);

        if (root->left->priority < root->priority) { 
            root = rotateRight(root);
        }
    } else if (val > root->value) {
        root->right = InsertTreap(val, pri, root->right);

        if (root->right->priority < root->priority) { 
            root = rotateLeft(root);
        }
    }

    return root;
}

// 3.PrintTreap
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
        bool hasNonNull = false; // 標記是否存在非空節點
        vector<string> levelOutput; // 暫存該層的輸出

        for (int i = 0; i < levelSize; ++i) {
            addr current = q.front();
            q.pop();

            if (current) {
                levelOutput.push_back(to_string(current->value) + "(" + to_string(current->priority) + ")");
                q.push(current->left);
                q.push(current->right);

                hasNonNull = true; // 該層有非空節點
            } else {
                levelOutput.push_back("null");
                q.push(nullptr); // 繼續放入 nullptr 保持完整結構
                q.push(nullptr);
            }
        }

        // 如果該層存在非空節點，才輸出
        if (hasNonNull) {
            for (const string& s : levelOutput) {
                cout << s << " ";
            }
            cout << endl;
        } else {
            break; // 當整層都是 null 時，停止
        }
    }
}


// 4.HeightTreap
int HeightTreap(addr root) {
    if (root == nullptr) {
        return 0;
    }
    return max(HeightTreap(root->left), HeightTreap(root->right)) + 1;
}

// 測試函數
void testTreap() {
    addr root = nullptr;

    root = InsertTreap(3, 0.9, root);
    root = InsertTreap(2, 0.5, root);
    root = InsertTreap(1, 0.3, root);
    root = InsertTreap(5, 0.2, root);
    root = InsertTreap(4, 0.1, root);

    // 列印 Treap
    PrintTreap(root);

    // 計算高度
    int height = HeightTreap(root);
    cout << "Height: " << height << endl;
}

int main(int argc, char *argv[]) {

    // testTreap(); 

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = pow(2, 20);
    int k_value = stoi(argv[1]); // k值

    const int numExperiments = 10; // 設定重複次數
    double totalDuration = 0;      // 記錄總時間
    double totalHeight = 0;     

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(0.0, 100000.0);

    addr out_root;

    for (int experiment = 0; experiment < numExperiments; experiment++) {
        int num = 1;
        std::vector<int> arr(n);
        for (int &x : arr)
            x = num++;
        uniform_int_distribution<> dis(0, n - 1);

        for (int i = 0; i < k_value; i++) {
            int random1 = dis(gen);
            int random2 = dis(gen);
            swap(arr[random1], arr[random2]);
        }
        auto start = chrono::high_resolution_clock::now();
        addr root = CreateTreap(arr[0], dist(gen));
        for (int i = 1; i < n; i++) {
            root = InsertTreap(arr[i], dist(gen), root);
        }
        auto end = chrono::high_resolution_clock::now();
        duration<double> duration = end - start;
        totalDuration += duration.count();
        totalHeight += HeightTreap(root);
    }
    double avgDuration = totalDuration / numExperiments;
    double avgHeight = totalHeight / numExperiments;

    cout << k_value << "," << avgDuration << "," << avgHeight << endl;
    return 0;
}
