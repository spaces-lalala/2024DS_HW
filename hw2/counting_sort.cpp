// counting_sort.cpp
#include <algorithm> // 加入這一行以使用 max_element
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

void counting_sort(std::vector<int> &arr) {
    int max_val = *std::max_element(arr.begin(), arr.end());
    std::vector<int> count(max_val + 1, 0);
    for (int x : arr)
        count[x]++;
    int index = 0;
    for (int i = 0; i <= max_val; ++i) {
        while (count[i]--)
            arr[index++] = i;
    }
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = stoi(argv[1]); // 讀取輸入的 n 值
    string mode = argv[2]; // 讀取輸入的 mode
    int k_value = stoi(argv[3]); // k值

    const int numExperiments = 10; // 設定重複次數
    double totalDuration_1 = 0;    // 記錄總時間
    double totalDuration_2 = 0;    // 記錄總時間
    double totalDuration_3 = 0;   // 記錄總時間

    srand(std::time(nullptr));

    if (mode == "first") {
        for (int experiment = 0; experiment < numExperiments; experiment++) {
            std::vector<int> arr(n);
            for (int &x : arr)
                x = rand() % n;
            auto start_1 = std::chrono::high_resolution_clock::now();
            counting_sort(arr);
            auto end_1 = std::chrono::high_resolution_clock::now();
            duration<double> duration_1 = end_1 - start_1;
            totalDuration_1 += duration_1.count();
        }
        double avgDuration_1 = totalDuration_1 / numExperiments;
        cout << n << "," << avgDuration_1  << endl;
    } else if (mode == "second") {
        for (int experiment = 0; experiment < numExperiments; experiment++) {
            int num = 1;
            std::vector<int> arr(n);
            for (int &x : arr)
                x = num++;
            for (int i = 0; i < k_value; i++) { //k次交換
                int random1 = rand() % n; 
                int random2 = rand() % n; 
                swap(arr[random1], arr[random2]); 
            }
            auto start_2 = std::chrono::high_resolution_clock::now();
            counting_sort(arr);
            auto end_2 = std::chrono::high_resolution_clock::now();
            duration<double> duration_2 = end_2 - start_2;
            totalDuration_2 += duration_2.count();
        }
        double avgDuration_2 = totalDuration_2 / numExperiments;
        cout << k_value << "," << avgDuration_2  << endl;
    }
    else{
        for (int experiment = 0; experiment < numExperiments; experiment++) {
            int num = 1;
            std::vector<int> arr(n);
            for (int &x : arr)
                x = rand() % k_value;
            auto start_3 = std::chrono::high_resolution_clock::now();
            counting_sort(arr);
            auto end_3 = std::chrono::high_resolution_clock::now();
            duration<double> duration_3 = end_3 - start_3;
            totalDuration_3 += duration_3.count();
        }
        double avgDuration_3 = totalDuration_3 / numExperiments;
        cout << k_value << "," << avgDuration_3  << endl;
    }
    return 0;
}
