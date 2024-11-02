// counting_sort.cpp
#include <iostream>
#include <vector>
#include <algorithm>  // 加入這一行以使用 max_element
#include <chrono>

void counting_sort(std::vector<int>& arr) {
    int max_val = *std::max_element(arr.begin(), arr.end());
    std::vector<int> count(max_val + 1, 0);
    for (int x : arr) count[x]++;
    int index = 0;
    for (int i = 0; i <= max_val; ++i) {
        while (count[i]--) arr[index++] = i;
    }
}

int main(int argc, char* argv[]) {
    int n = std::stoi(argv[1]);
    std::vector<int> arr(n);
    for (int& x : arr) x = rand() % n;

    auto start = std::chrono::high_resolution_clock::now();
    counting_sort(arr);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << duration.count() << std::endl;
    return 0;
}
