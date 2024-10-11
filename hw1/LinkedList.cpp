#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace std::chrono;

class LinkedList {
private:
    struct Node {
        int data;
        Node* next;
    };

    Node* head;
    Node* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    ~LinkedList() {
        Node* current = head;
        Node* nextNode;

        // 釋放鏈結串列的所有節點
        while (current != nullptr) {
            nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // Function to print the memory address of a node
    void printNodeAddress(Node* node) {
        cout << "Node address: " << node << endl;
    }

    // 新增節點到尾端
    void add(int value) {
        // 動態分配新節點
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = nullptr;

        // 打印新節點的記憶體地址
        // printNodeAddress(newNode);

        // 如果鏈表是空的，直接新增第一個節點
        if (head == nullptr) {
            head = tail = newNode;  // 初始化 head 和 tail
            // cout << "Inserted first node." << endl;
        } else {
            // 將新節點連接到當前的尾節點後面
            tail->next = newNode;
            tail = newNode;  // 更新 tail 指向新的最後一個節點
            // cout << "Inserted node at tail." << endl;
        }
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

    // 列印鏈結串列中的所有節點
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

    int n;

    string mode = argv[2]; // 讀取輸入的 mode

    if (mode == "third") n = int(pow(2,20));
    
    else n = stoi(argv[1]); // 讀取輸入的 n 值

    const int block_size = int(pow(2,13));
    

    const int numExperiments = 10; // 設定重複次數
    double totalDuration_1 = 0; // 記錄總時間
    double totalDuration_2 = 0; // 記錄總時間
    double totalDuration_3[150]; // 記錄總時間

    for (int i = 0; i < 150; i++){
        totalDuration_3[i] = 0;
    }

    for (int experiment = 0; experiment < numExperiments; experiment++) {
        LinkedList linkedList;
        int count3 = 0;

        // 開始計時
        auto start_1 = high_resolution_clock::now();

        // 新增 n 筆隨機資料
        for (int i = 0; i < n; i++) {
            if ((i+1)%block_size == 0){ //mode = "third"
                auto start_3 = high_resolution_clock::now();
                int value = rand() % 10000;
                linkedList.add(value);
                auto end_3 = high_resolution_clock::now();
                duration<double> duration_3 = end_3 - start_3;
                totalDuration_3[count3] += duration_3.count();
            }
            else{
                int value = rand() % 10000;
                linkedList.add(value);
            }
        }

        // 結束計時 mode == "first"
        auto end_1 = high_resolution_clock::now();
        auto duration_1 = duration_cast<microseconds>(end_1 - start_1);
        totalDuration_1 += duration_1.count(); 

        // 計算總和計時 Mode == "second"
        auto Start_2 = high_resolution_clock::now();
        // int totalSum_2 = linkedList.sum();
        linkedList.sum_void();
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
