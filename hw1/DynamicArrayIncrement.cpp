#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace std::chrono;

// 動態陣列類別
class   DynamicArrayIncrement {
private:
    int* arr;       // 指向陣列的指標
    size_t capacity; // 當前容量
    size_t size;     // 當前元素個數

    // 擴充容量的方法，每次將容量加 1
    void resize() {
        capacity += 1;                          // 每次擴充時僅增加 1
        int* newArr = new int[capacity];        // 分配新的更大陣列
        for (size_t i = 0; i < size; i++) {
            newArr[i] = arr[i];                 // 拷貝舊陣列的內容
        }
        delete[] arr;                           // 釋放舊陣列
        arr = newArr;                           // 將陣列指標指向新陣列
    }

public:
    // 建構子：初始化陣列，預設容量為 1
      DynamicArrayIncrement() {
        capacity = 1;
        size = 0;
        arr = new int[capacity];
    }

    // 解構子：釋放記憶體
    ~DynamicArrayIncrement() {
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

    // 計算陣列內所有元素的總和
    int sum() const {
        int total = 0;
        for (size_t i = 0; i < size; i++) {
            total += arr[i];
        }
        return total;
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


int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = stoi(argv[1]); // 讀取輸入的 n 值
    string mode = argv[2]; // 讀取輸入的 mode

    const int numExperiments = 10; // 設定重複次數
    double totalDuration_1 = 0; // 記錄總時間
    double totalDuration_2 = 0; // 記錄總時間
    double totalDuration_3 = 0; // 記錄總時間

    for (int experiment = 0; experiment < numExperiments; experiment++) {
        DynamicArrayIncrement dynamicArray;

        // 開始計時
        auto start_1 = high_resolution_clock::now();

        // 新增 n 筆隨機資料
        for (int i = 0; i < n; i++) {
            int value = rand() % 10000;
            dynamicArray.add(value);
        }

        // 結束計時 mode=="first"
        auto end_1 = high_resolution_clock::now();
        auto duration_1 = duration_cast<microseconds>(end_1 - start_1);
        totalDuration_1 += duration_1.count();  

        //mode == "second" 
        auto Start_2 = high_resolution_clock::now();
        int totalSum_2 = dynamicArray.sum();
        auto End_2 = high_resolution_clock::now();
        duration<double> sumDuration_2 = End_2 - Start_2;
        totalDuration_2 += sumDuration_2.count();
    }
    // 輸出結果
    double avgDuration_1 = totalDuration_1 / numExperiments;
    double avgDuration_2 = totalDuration_2 / numExperiments; // mode == "second"
    if(mode == "first" || mode == "first_2" )     cout << n << "," << avgDuration_1 / 1e6 << endl; // 輸出資料數量和所需時間（秒） 
    else if(mode == "second" || mode == "second_2" ) cout << n << "," << avgDuration_2 / 1e6 << endl; // 輸出資料數量和所需時間（秒）

    return 0;
}
