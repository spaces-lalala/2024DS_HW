// quick_merge_sort.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

int hoare_partition(std::vector<int>& arr, int low, int high) {
    int random_index = low + rand() % (high - low + 1);
    std::swap(arr[low], arr[random_index]); 
    int pivot = arr[low];
    int i = low - 1, j = high + 1;
    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        std::swap(arr[i], arr[j]);
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void quick_merge_sort(std::vector<int>& arr, int low, int high, int s) {
    if (high - low + 1 <= s) {
        merge_sort(arr, low, high);
    } else if (low < high) {
        int p = hoare_partition(arr, low, high);
        quick_merge_sort(arr, low, p, s);
        quick_merge_sort(arr, p + 1, high, s);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = stoi(argv[1]); // 讀取輸入的 n 值
    string mode = argv[2]; // 讀取輸入的 mode
    int k_value = stoi(argv[3]); // k值
    int s = stoi(argv[4]); // s值
    s = pow(10, s);

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
            quick_merge_sort(arr, 0, n - 1, s);
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
            quick_merge_sort(arr, 0, n - 1, s);
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
            quick_merge_sort(arr, 0, n - 1, s);
            auto end_3 = std::chrono::high_resolution_clock::now();
            duration<double> duration_3 = end_3 - start_3;
            totalDuration_3 += duration_3.count();
        }
        double avgDuration_3 = totalDuration_3 / numExperiments;
        cout << k_value << "," << avgDuration_3  << endl;
    }
    return 0;
}
