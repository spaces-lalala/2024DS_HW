#include <iostream>
#include <vector>
#include <chrono> // For timing
#include <cstdlib> // For random numbers
#include <cmath>   // For pow()

using namespace std;
using namespace std::chrono;

// 動態陣列類別
class DynamicArray {
private:
    int* arr;       // 指向陣列的指標
    size_t capacity; // 當前容量
    size_t size;     // 當前元素個數

    // 擴充容量的方法，每次將容量加倍
    void resize() {
        capacity *= 2;                          // 將容量加倍
        int* newArr = new int[capacity];        // 分配新的更大陣列
        for (size_t i = 0; i < size; i++) {
            newArr[i] = arr[i];                 // 拷貝舊陣列的內容
        }
        delete[] arr;                           // 釋放舊陣列
        arr = newArr;                           // 將陣列指標指向新陣列
    }

public:
    // 建構子：初始化陣列，預設容量為 1
    DynamicArray() {
        capacity = 1;
        size = 0;
        arr = new int[capacity];
    }

    // 解構子：釋放記憶體
    ~DynamicArray() {
        delete[] arr;
    }

    // 新增元素
    void add(int value) {
        if (size == capacity) {
            resize();  // 當容量不足時，擴充容量
        }
        arr[size] = value;
        size++;
    }

    // 取得元素數量
    size_t getSize() const {
        return size;
    }

    // 取得指定索引的元素
    int get(int index) const {
        if (index >= 0 && index < size) {
            return arr[index];
        } else {
            cerr << "Index out of bounds!" << endl;
            return -1; // 若索引超出範圍則回傳 -1
        }
    }

    // 取得目前的容量
    size_t getCapacity() const {
        return capacity;
    }
};

// 主程式
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <n>\n";
        return 1;
    }

    int n = stoi(argv[1]); // 讀取輸入的 n 值

    const int numExperiments = 10; // 設定重複次數
    double totalDuration = 0; // 記錄總時間

    for (int experiment = 0; experiment < numExperiments; experiment++) {
        DynamicArray dynamicArray;

        // 開始計時
        auto start = high_resolution_clock::now();

        // 新增 n 筆隨機資料
        for (int i = 0; i < n; i++) {
            int value = rand() % 10000; // 隨機數字 0-9999
            dynamicArray.add(value);
        }

        // 結束計時
        auto end = high_resolution_clock::now();
        duration<double> duration = end - start;
        totalDuration += duration.count();
    }

    // 計算平均時間
    double avgDuration = totalDuration / numExperiments;
    cout << n << "," << avgDuration / 1e6 << endl; // 輸出資料數量和所需時間（秒） 


    return 0;
}
