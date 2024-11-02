// insertion_sort.cpp
#include <iostream>
#include <vector>
#include <chrono>

void insertion_sort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

int main(int argc, char* argv[]) {
    int n = std::stoi(argv[1]);
    std::vector<int> arr(n);
    for (int& x : arr) x = rand() % n;
    
    auto start = std::chrono::high_resolution_clock::now();
    insertion_sort(arr);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;
    return 0;
}
