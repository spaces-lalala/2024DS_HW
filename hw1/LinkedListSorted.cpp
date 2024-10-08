#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

class LinkedListSorted {
private:
    struct Node {
        int data;
        Node* next;
    };

    Node* head;

    // 根據記憶體地址對節點進行排序
    void sortByMemoryAddress(vector<Node*>& nodes) {
        sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {
            return a < b; // 按記憶體地址排序
        });
    }

public:
    LinkedListSorted() : head(nullptr) {}

    ~LinkedListSorted() {
        Node* current = head;
        Node* nextNode;
        while (current != nullptr) {
            nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void add(int value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = head;
        head = newNode;
    }

    void sortListByAddress() {
        vector<Node*> nodes;
        Node* current = head;

        // 收集所有節點
        while (current != nullptr) {
            nodes.push_back(current);
            current = current->next;
        }

        // 根據記憶體地址排序
        sortByMemoryAddress(nodes);

        // 重新連結節點
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (i + 1 < nodes.size()) {
                nodes[i]->next = nodes[i + 1];
            } else {
                nodes[i]->next = nullptr;
            }
        }

        head = nodes.empty() ? nullptr : nodes[0];
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <n>\n";
        return 1;
    }

    int n = stoi(argv[1]);
    const int numExperiments = 10; // 設定重複次數
    double totalDuration = 0; // 記錄總時間

    for (int experiment = 0; experiment < numExperiments; experiment++) {
    LinkedListSorted linkedList;

    // 開始計時
    auto start = high_resolution_clock::now();

    // 新增 n 筆隨機資料
    for (int i = 0; i < n; i++) {
        int value = rand() % 10000;
        linkedList.add(value);
    }

    // 對鏈結串列進行排序
    linkedList.sortListByAddress();

    auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        totalDuration += duration.count();  
    }
    // 輸出結果
    double avgDuration = totalDuration / numExperiments;
    cout << n << "," << avgDuration / 1e6 << endl; // 輸出資料數量和所需時間（秒） 

    return 0;
}
