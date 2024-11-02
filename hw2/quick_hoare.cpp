// quick_sort_hoare.cpp
#include <iostream>
#include <vector>
#include <chrono>

int hoare_partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[low];
    int i = low - 1, j = high + 1;
    while (true) {
        do { ++i; } while (arr[i] < pivot);
        do { --j; } while (arr[j] > pivot);
        if (i >= j) return j;
        std::swap(arr[i], arr[j]);
    }
}

void quick_sort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int p = hoare_partition(arr, low, high);
        quick_sort(arr, low, p);
        quick_sort(arr, p + 1, high);
    }
}

int main(int argc, char* argv[]) {
    int n = std::stoi(argv[1]);
    std::vector<int> arr(n);
    for (int& x : arr) x = rand() % n;

    auto start = std::chrono::high_resolution_clock::now();
    quick_sort(arr, 0, n - 1);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;
    return 0;
}
