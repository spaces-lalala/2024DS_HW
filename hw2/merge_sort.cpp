// merge_sort.cpp
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

void merge(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(std::vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    merge_sort(arr, l, m);
    merge_sort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int main(int argc, char* argv[]) {
    
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <n> <mode>\n";
        return 1;
    }

    int n = stoi(argv[1]); // 讀取輸入的 n 值
    string mode = argv[2]; // 讀取輸入的 mode
    int k = stoi(argv[3]); // k值

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
            merge_sort(arr, 0, n - 1);
            auto end_1 = std::chrono::high_resolution_clock::now();
            duration<double> duration_1 = end_1 - start_1;
            totalDuration_1 += duration_1.count();
        }
        double avgDuration_1 = totalDuration_1 / numExperiments;
        cout << n << "," << avgDuration_1 / 1e6 << endl;
    } else if (mode == "second") {
        for (int experiment = 0; experiment < numExperiments; experiment++) {
            int num = 1;
            std::vector<int> arr(n);
            for (int &x : arr)
                x = num++;
            for (int i = 0; i < k; i++) { //k次交換
                int random1 = rand() % n; 
                int random2 = rand() % n; 
                swap(arr[random1], arr[random2]); 
            }
            auto start_2 = std::chrono::high_resolution_clock::now();
            merge_sort(arr, 0, n - 1);
            auto end_2 = std::chrono::high_resolution_clock::now();
            duration<double> duration_2 = end_2 - start_2;
            totalDuration_2 += duration_2.count();
        }
        double avgDuration_2 = totalDuration_2 / numExperiments;
        cout << n << "," << avgDuration_2 / 1e6 << endl;
    }
    else{
        for (int experiment = 0; experiment < numExperiments; experiment++) {
            int num = 1;
            std::vector<int> arr(n);
            for (int &x : arr)
                x = rand() % k;
            auto start_3 = std::chrono::high_resolution_clock::now();
            merge_sort(arr, 0, n - 1);
            auto end_3 = std::chrono::high_resolution_clock::now();
            duration<double> duration_3 = end_3 - start_3;
            totalDuration_3 += duration_3.count();
        }
        double avgDuration_3 = totalDuration_3 / numExperiments;
        cout << n << "," << avgDuration_3 / 1e6 << endl;
    }
    return 0;
}
