// quick_3way.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdlib>

void quick_sort_3way(std::vector<int>& arr, int low, int high) {
    if (low >= high) return;
    int pivot_index = low + rand() % (high - low + 1);
    std::swap(arr[low], arr[pivot_index]);
    int pivot = arr[low];
    
    int lt = low, gt = high, i = low + 1;
    while (i <= gt) {
        if (arr[i] < pivot) std::swap(arr[i++], arr[lt++]);
        else if (arr[i] > pivot) std::swap(arr[i], arr[gt--]);
        else i++;
    }

    quick_sort_3way(arr, low, lt - 1);
    quick_sort_3way(arr, gt + 1, high);
}

int main(int argc, char* argv[]) {
    int n = std::stoi(argv[1]);
    std::vector<int> arr(n);
    for (int& x : arr) x = rand() % n;

    auto start = std::chrono::high_resolution_clock::now();
    quick_sort_3way(arr, 0, n - 1);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;
    return 0;
}
