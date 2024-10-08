#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

class LinkedList {
private:
    struct Node {
        int data;
        Node* next;
    };

    Node* head;

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
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
        LinkedList linkedList;

        // 開始計時
        auto start = high_resolution_clock::now();

        // 新增 n 筆隨機資料
        for (int i = 0; i < n; i++) {
            int value = rand() % 10000;
            linkedList.add(value);
        }

        // 結束計時
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        totalDuration += duration.count();  
    }
    // 輸出結果
    double avgDuration = totalDuration / numExperiments;
    cout << n << "," << avgDuration / 1e6 << endl; // 輸出資料數量和所需時間（秒） 

    return 0;
}
