// quick_merge_sort.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>

int hoare_partition(std::vector<int>& arr, int low, int high) {
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
    int n = std::stoi(argv[1]);
    int s = std::stoi(argv[2]);
    std::vector<int> arr(n);
    for (int& x : arr) x = rand() % n;

    auto start = std::chrono::high_resolution_clock::now();
    quick_merge_sort(arr, 0, n - 1, s);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;
    return 0;
}
