// quick_lomuto.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdlib>

int lomuto_partition(std::vector<int>& arr, int low, int high) {
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
    int n = std::stoi(argv[1]);
    std::vector<int> arr(n);
    for (int& x : arr) x = rand() % n;

    auto start = std::chrono::high_resolution_clock::now();
    quick_sort_lomuto(arr, 0, n - 1);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;
    return 0;
}
