#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <map>
#include <unordered_map>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <k>" << endl;
        return 1;
    }

    int k = stoi(argv[1]);
    long long n = 1LL << k;  // 2^k
    const long long MAX_VAL = 1LL << 30;  // 2^30
    const int NUM_TRIALS = 20; // Number of trials

    // Use proper random number generation
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dist(1, MAX_VAL);

    double hash_insert_total_time = 0.0;
    double hash_search_total_time = 0.0;
    double bst_insert_total_time = 0.0;
    double bst_search_total_time = 0.0;

    for (int trial = 0; trial < NUM_TRIALS; ++trial) {
        // Generate random data for each trial
        vector<long long> data(n);
        for (long long i = 0; i < n; i++) {
            data[i] = dist(gen);
        }

        vector<long long> search_data(100000);
        for (int i = 0; i < 100000; i++) {
            search_data[i] = dist(gen);
        }

        // Hash table insertion
        unordered_map<long long, int> hash_table;
        auto hash_insert_start = high_resolution_clock::now();
        for (long long i = 0; i < n; i++) {
            hash_table.insert({data[i], i});
        }
        auto hash_insert_end = high_resolution_clock::now();
        hash_insert_total_time += duration_cast<milliseconds>(hash_insert_end - hash_insert_start).count() / 1000.0;

        // Hash table search
        auto hash_search_start = high_resolution_clock::now();
        for (int i = 0; i < 100000; i++) {
            hash_table.find(search_data[i]);
        }
        auto hash_search_end = high_resolution_clock::now();
        hash_search_total_time += duration_cast<milliseconds>(hash_search_end - hash_search_start).count() / 1000.0;

        // BST insertion
        map<long long, int> bst;
        auto bst_insert_start = high_resolution_clock::now();
        for (long long i = 0; i < n; i++) {
            bst.insert({data[i], i});
        }
        auto bst_insert_end = high_resolution_clock::now();
        bst_insert_total_time += duration_cast<milliseconds>(bst_insert_end - bst_insert_start).count() / 1000.0;

        // BST search
        auto bst_search_start = high_resolution_clock::now();
        for (int i = 0; i < 100000; i++) {
            bst.find(search_data[i]);
        }
        auto bst_search_end = high_resolution_clock::now();
        bst_search_total_time += duration_cast<milliseconds>(bst_search_end - bst_search_start).count() / 1000.0;
    }

    // Calculate averages
    double hash_insert_time = hash_insert_total_time / NUM_TRIALS;
    double hash_search_time = hash_search_total_time / NUM_TRIALS;
    double bst_insert_time = bst_insert_total_time / NUM_TRIALS;
    double bst_search_time = bst_search_total_time / NUM_TRIALS;

    // Output in CSV format
    cout << n << ","
         << hash_insert_time << ","
         << hash_search_time << ","
         << bst_insert_time << ","
         << bst_search_time << endl;

    return 0;
}
