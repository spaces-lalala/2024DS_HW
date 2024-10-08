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
    Node* tail;
    void* memoryBlock;  // 保存整塊預先分配的記憶體區域
    size_t currentOffset;  // 用於跟踪已使用的記憶體區域
    size_t totalNodes;  // 總共能分配的節點數

public:
    LinkedListSorted(size_t maxNodes) : head(nullptr), tail(nullptr), currentOffset(0), totalNodes(maxNodes) {
        // 預先分配足夠大的連續記憶體區域
        memoryBlock = malloc(maxNodes * sizeof(Node));
        if (memoryBlock == nullptr) {
            cerr << "Memory allocation failed!" << endl;
            exit(1);
        }
    }

    ~LinkedListSorted() {
        // 使用 malloc 分配的記憶體需要使用 free 釋放
        free(memoryBlock);
    }

    // Function to print the memory address of a node
    void printNodeAddress(Node* node) {
        cout << "Node address: " << node << endl;
    }


    // 自動從預先分配的區域內分配記憶體給新節點
    Node* allocateNode() {
        if (currentOffset >= totalNodes) {
            cerr << "No more memory to allocate nodes!" << endl;
            return nullptr;  // 超過分配範圍
        }

        // 根據當前偏移量從預先分配的區域中提取記憶體
        Node* newNode = reinterpret_cast<Node*>(reinterpret_cast<char*>(memoryBlock) + currentOffset * sizeof(Node));
        currentOffset++;  // 更新偏移量，指向下一個可分配的位置
        return newNode;
    }

    

    void add(int value) {
        // 分配新節點
        Node* newNode = allocateNode();
        if (newNode == nullptr) {
            return;  // 無法再分配新節點
        }

        newNode->data = value;
        newNode->next = nullptr;

        printNodeAddress(newNode);

        // 如果列表是空的，直接添加第一個節點
        if (head == nullptr) {
            head = tail = newNode;  // 初始化 head 和 tail
            printNodeAddress(newNode);  // 立即打印地址
            // cout << "Inserted first node." << endl;
            return;
        }

        // 確保新節點地址比 tail 節點地址大（因為我們是按順序分配的，所以自動滿足）
        tail->next = newNode;
        tail = newNode;  // 更新 tail 指向新的最後一個節點

        // cout << "Inserted node with larger memory address." << endl;
    }

    // 列印鏈結串列
    void printList() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};



int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <n>\n";
        return 1;
    }

    int n = stoi(argv[1]);
    const int numExperiments = 1; // 設定重複次數
    double totalDuration = 0; // 記錄總時間

    for (int experiment = 0; experiment < numExperiments; experiment++) {
        cout << "New" << endl;
        LinkedListSorted LinkedListSorted(n);

        // 開始計時
        auto start = high_resolution_clock::now();

        // 新增 n 筆隨機資料
        for (int i = 0; i < n; i++) {
            int value = rand() % 10000;
            LinkedListSorted.add(value);
        }

        auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            totalDuration += duration.count();  
    }
    // 輸出結果
    double avgDuration = totalDuration / numExperiments;
    cout << n << "," << avgDuration / 1e6 << endl; // 輸出資料數量和所需時間（秒） 

    return 0;
}
