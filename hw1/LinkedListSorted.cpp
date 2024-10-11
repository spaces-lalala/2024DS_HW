#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cmath>

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
            cerr << currentOffset << " " << totalNodes << endl;
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

        // printNodeAddress(newNode);

        // 如果列表是空的，直接添加第一個節點
        if (head == nullptr) {
            head = tail = newNode;  // 初始化 head 和 tail
            // printNodeAddress(newNode);  // 立即打印地址
            // cout << "Inserted first node." << endl;
            return;
        }

        // 確保新節點地址比 tail 節點地址大（因為我們是按順序分配的，所以自動滿足）
        tail->next = newNode;
        tail = newNode;  // 更新 tail 指向新的最後一個節點

        // cout << "Inserted node with larger memory address." << endl;
    }

    int sum() const {
        int total = 0;
        Node* current = head;
        while (current != nullptr) {
            total += current->data;
            current = current->next;
        }
        return total;
    }

    //用來計算時間
    void sum_void() const {
        int total = 0;
        Node* current = head;
        while (current != nullptr) {
            total = current->data;
            current = current->next;
        }
        return ;
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
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = stoi(argv[1]);
    string mode = argv[2]; // 讀取輸入的 mode

    if (mode == "third") n = int(pow(2,20));

    const int block_size = int(pow(2,13));

    const int numExperiments = 10; // 設定重複次數
    double totalDuration_1 = 0; // 記錄總時間
    double totalDuration_2 = 0; // 記錄總時間
    double totalDuration_3[150]; // 記錄總時間

    for (int i = 0; i < 150; i++){
        totalDuration_3[i] = 0;
    }

    for (int experiment = 0; experiment < numExperiments; experiment++) {
        // cout << "New" << endl;
        LinkedListSorted LinkedListSorted(n);
        int count3 = 0;

        // 開始計時
        auto start_1 = high_resolution_clock::now();

        // 新增 n 筆隨機資料
        for (int i = 0; i < n; i++) {
            if ((i+1)%block_size == 0){ //mode == "third"
                auto start_3 = high_resolution_clock::now();
                int value = rand() % 10000;
                LinkedListSorted.add(value);
                auto end_3 = high_resolution_clock::now();
                duration<double> duration_3 = end_3 - start_3;
                totalDuration_3[count3] += duration_3.count();
            }
            else{
                int value = rand() % 10000;
                LinkedListSorted.add(value);
            }
        }


        // 結束計時 mode == "first"
        auto end_1 = high_resolution_clock::now();
        auto duration_1 = duration_cast<microseconds>(end_1 - start_1);
        totalDuration_1 += duration_1.count();  

        
        // 計算總和計時 Mode == "second"
        auto Start_2 = high_resolution_clock::now();
        // int totalSum_2 = LinkedListSorted.sum();
        LinkedListSorted.sum_void();
        auto End_2 = high_resolution_clock::now();
        auto sumDuration_2 = duration_cast<microseconds>(End_2 - Start_2);
        totalDuration_2 += sumDuration_2.count(); 

    }
    // 輸出結果
    double avgDuration_1 = totalDuration_1 / numExperiments;
    double avgDuration_2 = totalDuration_2 / numExperiments; // mode == "second"
    double avgDuration_3[150]; // mode == "third"

    for (int i = 0; i < 128; i++){
        avgDuration_3[i] = totalDuration_3[i] / numExperiments;
    }

    if(mode == "first" || mode == "first_2" )     cout << n << "," << avgDuration_1 / 1e6 << endl; // 輸出資料數量和所需時間（秒） 
    else if(mode == "second" || mode == "second_2" ) cout << n << "," << avgDuration_2 / 1e6 << endl; // 輸出資料數量和所需時間（秒）
    else if(mode == "third") {
        for (int i = 0; i < 128; i++){
            cout << i+1 << "," << avgDuration_3[i] / 1e6 << endl; // 輸出資料數量和所需時間（秒）
        }
    }

    return 0;
}
