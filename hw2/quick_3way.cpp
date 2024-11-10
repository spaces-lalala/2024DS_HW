// quick_3way.cpp
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

void way3_partition(std::vector<int>& arr, int l, int r, int &i, int &j) {
    i = l - 1, j = r;
    int p = l - 1, q = r;
    int v = arr[r];

    while (true) {
        while (arr[++i] < v);

        while (v < arr[--j])
            if (j == l)
                break;

        if (i >= j)
            break;

        swap(arr[i], arr[j]);

        if (arr[i] == v) {
            p++;
            swap(arr[p], arr[i]);
        }

        if (arr[j] == v) {
            q--;
            swap(arr[j], arr[q]);
        }
    }
    swap(arr[i], arr[r]);

    j = i - 1;
    for (int k = l; k < p; k++, j--)
        swap(arr[k], arr[j]);

    i = i + 1;
    for (int k = r - 1; k > q; k--, i++)
        swap(arr[i], arr[k]);
}

// 3-way 3way_partition based quick sort
void quick_sort_3way(std::vector<int>& arr, int l, int r) {
    if (r <= l)
        return;

    int i, j;

    way3_partition(arr, l, r, i, j);

    // Recur
    quick_sort_3way(arr, l, j);
    quick_sort_3way(arr, i, r);
}

int main(int argc, char *argv[]) {

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
    double totalDuration_3 = 0;    // 記錄總時間

    srand(std::time(nullptr));

    if (mode == "first") {
        for (int experiment = 0; experiment < numExperiments; experiment++) {
            std::vector<int> arr(n);
            for (int &x : arr)
                x = rand() % n;
            auto start_1 = std::chrono::high_resolution_clock::now();
            quick_sort_3way(arr, 0, n - 1);
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
            for (int i = 0; i < k; i++) { // k次交換
                int random1 = rand() % n;
                int random2 = rand() % n;
                swap(arr[random1], arr[random2]);
            }
            auto start_2 = std::chrono::high_resolution_clock::now();
            quick_sort_3way(arr, 0, n - 1);
            auto end_2 = std::chrono::high_resolution_clock::now();
            duration<double> duration_2 = end_2 - start_2;
            totalDuration_2 += duration_2.count();
        }
        double avgDuration_2 = totalDuration_2 / numExperiments;
        cout << n << "," << avgDuration_2 / 1e6 << endl;
    } else {
        for (int experiment = 0; experiment < numExperiments; experiment++) {
            int num = 1;
            std::vector<int> arr(n);
            for (int &x : arr)
                x = rand() % k;
            auto start_3 = std::chrono::high_resolution_clock::now();
            quick_sort_3way(arr, 0, n - 1);
            auto end_3 = std::chrono::high_resolution_clock::now();
            duration<double> duration_3 = end_3 - start_3;
            totalDuration_3 += duration_3.count();
        }
        double avgDuration_3 = totalDuration_3 / numExperiments;
        cout << n << "," << avgDuration_3 / 1e6 << endl;
    }
    return 0;
}
