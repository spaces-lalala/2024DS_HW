// SkipList.cpp
#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
#include <queue>
#include <vector>
#include <random>

using namespace std;
using namespace std::chrono;

class Node {
  public:
    int key;
    bool isRandom;
    int level;

    // Array to hold pointers to node of different level
    Node **forward;
    Node(int, int, bool);
};

Node::Node(int key, int level, bool isRandom) {
    this->key = key;
    this->level = level;
    this->isRandom = isRandom;

    // Allocate memory to forward
    forward = new Node *[level + 1];

    // Fill forward array with 0(NULL)
    memset(forward, 0, sizeof(Node *) * (level + 1));
};

// Class for Skip list
class SkipList {
    // Maximum level for this skip list
    int MAXLVL;

    // P is the fraction of the nodes with level
    // i pointers also having level i+1 pointers
    float P;

    // current level of skip list
    int level;

  public:
    SkipList(int, float);
    int randomLevel();
    Node *createNode(int, int, bool);
    void InsertSkipList(int, bool useRandom = true, int rlevel = 0);
    void PrintSkipList();
    int HeightSkipList(Node *root);
    // pointer to header node
    Node *header;
};

SkipList::SkipList(int MAXLVL, float P) {
    this->MAXLVL = MAXLVL;
    this->P = P;
    level = 0;

    // create header node and initialize key to -1
    header = new Node(-1, MAXLVL, true);
};

// create random level for node
int SkipList::randomLevel() {
    float r = (float)rand() / RAND_MAX;
    int lvl = 0;
    while (r < P && lvl < MAXLVL) {
        lvl++;
        r = (float)rand() / RAND_MAX;
    }
    return lvl;
};

// create new node
Node *SkipList::createNode(int key, int level, bool isRandom) {
    Node *n = new Node(key, level, isRandom);
    return n;
};

// 2.InsertSkipList
void SkipList::InsertSkipList(int key, bool useRandom, int rlevel) {
    if (useRandom) {
        rlevel = randomLevel();
    }

    Node *current = header;

    // create update array and initialize it
    Node *update[MAXLVL + 1];
    memset(update, 0, sizeof(Node *) * (MAXLVL + 1));

    for (int i = level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key)
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];

    if (current == NULL || current->key != key) {
        if (rlevel > level) {
            for (int i = level + 1; i < rlevel + 1; i++)
                update[i] = header;

            level = rlevel;
        }

        Node *n = createNode(key, rlevel, useRandom);

        for (int i = 0; i <= rlevel; i++) {
            n->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = n;
        }
        // cout << "Successfully Inserted key " << key << " with level " << rlevel
        //      << "\n";
    }
};

// 3.PrintSkipList
void SkipList::PrintSkipList() {
    // cout << "\n*****Skip List*****" << "\n";
    for (int i = 0; i <= level; i++) {
        Node *node = header->forward[i];
        cout << "Level " << i << ": ";
        while (node != NULL) {
            cout << node->key << " ";
            node = node->forward[i];
        }
        cout << "\n";
    }
};

// 4.HeightSkipList
int SkipList::HeightSkipList(Node *root) {
    if (!root)
        return 0;
    int height = 0;
    for (int i = MAXLVL; i >= 0; i--) {
        if (root->forward[i] != NULL) {
            height = i + 1;
            break;
        }
    }
    return height;
}

// 測試函數
void testSkipList() {
    SkipList lst(100, 0.5);

    // 插入指定層級的節點
    lst.InsertSkipList(1, false, 2); // HHT
    lst.InsertSkipList(2, false, 0); // T
    lst.InsertSkipList(3, false, 1); // HT
    lst.InsertSkipList(4, false, 3); // HHHT
    lst.InsertSkipList(5, false, 0); // T
    lst.InsertSkipList(6, false, 0); // T
    lst.InsertSkipList(7, false, 2); // HHT
    lst.PrintSkipList();

    // Test HeightSkipList
    cout << "Height of SkipList: " << lst.HeightSkipList(lst.header) << endl;
};

int main(int argc, char *argv[]) {
    // testSkipList();

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = pow(2, 20);
    int k_value = stoi(argv[1]); // k值

    const int numExperiments = 10; // 設定重複次數
    double totalDuration = 0;      // 記錄總時間

    srand(std::time(nullptr));

    double height=0;

    std::random_device rd;
    std::mt19937 gen(rd());


    for (int experiment = 0; experiment < numExperiments; experiment++) {
        SkipList lst(1000, 0.5);
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
        lst.InsertSkipList(arr[0]);
        for (int i = 0; i < n; i++) {
            lst.InsertSkipList(arr[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        duration<double> duration = end - start;
        totalDuration += duration.count();
        height += lst.HeightSkipList(lst.header);
    }
    double avgDuration = totalDuration / numExperiments;
    double avgHeight = height/numExperiments;
    cout << k_value << "," << avgDuration << "," << avgHeight << endl;

    return 0;
}
