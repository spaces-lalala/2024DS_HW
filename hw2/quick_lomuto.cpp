// quick_lomuto.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

int lomuto_partition(std::vector<int>& arr, int low, int high) {
    int random_index = low + rand() % (high - low + 1);
    std::swap(arr[random_index], arr[high]);
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quick_sort_lomuto(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot_index = low + rand() % (high - low + 1);
        std::swap(arr[pivot_index], arr[high]);
        int p = lomuto_partition(arr, low, high);
        quick_sort_lomuto(arr, low, p - 1);
        quick_sort_lomuto(arr, p + 1, high);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
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
            quick_sort_lomuto(arr, 0, n - 1);
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
            quick_sort_lomuto(arr, 0, n - 1);
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
            quick_sort_lomuto(arr, 0, n - 1);
            auto end_3 = std::chrono::high_resolution_clock::now();
            duration<double> duration_3 = end_3 - start_3;
            totalDuration_3 += duration_3.count();
        }
        double avgDuration_3 = totalDuration_3 / numExperiments;
        cout << k_value << "," << avgDuration_3  << endl;
    }
    return 0;
}