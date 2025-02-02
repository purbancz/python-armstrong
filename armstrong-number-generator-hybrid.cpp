#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <omp.h>

using namespace std;
using ull = unsigned long long;
using uint128 = __uint128_t; // unsigned 128-bit integer

// Helper function to print __int128 as a string
void print_uint128(uint128 num) {
    if (num == 0) {
        cout << "0";
        return;
    }

    string result;
    while (num > 0) {
        result.push_back('0' + (num % 10));
        num /= 10;
    }
    reverse(result.begin(), result.end());
    cout << result;
}

// Helper function to determine if we should use __int128
bool needs_uint128(ull num) {
    return num > numeric_limits<ull>::max() / 10; // Rough threshold for switching
}

/**
 * Recursively generate and process combinations (with replacement) of digits.
 */
template <typename T>
void process_combinations(int remaining, int start, vector<int>& comb, int length,
                          const vector<vector<T>>& powers, T upper_bound, T lower_bound,
                          set<T>& results) {
    if (remaining == 0) {
        // Compute candidate = sum_{d in comb} (d^length)
        T candidate = 0;
        for (int d : comb) {
            candidate += powers[length][d];
        }
        // Discard if candidate is outside the valid range.
        if (candidate > upper_bound) return;
        if (length > 1 && candidate < lower_bound) return;
        
        // Convert candidate to its sorted digits.
        vector<int> candidate_digits;
        T temp = candidate;
        if (temp == 0) {
            candidate_digits.push_back(0);
        } else {
            while (temp > 0) {
                candidate_digits.push_back((int)(temp % 10));
                temp /= 10;
            }
        }
        sort(candidate_digits.begin(), candidate_digits.end());
        
        // The combination 'comb' is already sorted.
        if (candidate_digits.size() != comb.size()) return;
        bool match = true;
        for (size_t i = 0; i < comb.size(); i++) {
            if (candidate_digits[i] != comb[i]) {
                match = false;
                break;
            }
        }
        if (match) {
            results.insert(candidate);
        }
        return;
    }
    
    for (int d = start; d <= 9; d++) {
        comb.push_back(d);
        process_combinations(remaining - 1, d, comb, length, powers, upper_bound, lower_bound, results);
        comb.pop_back();
    }
}

/**
 * Generate Armstrong (narcissistic) numbers with up to 'ran' digits.
 */
template <typename T>
vector<T> armstrong(int ran) {
    // Precompute powers: powers[i][d] = d^i for i in 0..ran and d in 0..9.
    vector<vector<T>> powers(ran + 1, vector<T>(10, 0));
    for (int i = 0; i <= ran; i++) {
        for (int d = 0; d < 10; d++) {
            T p = 1;
            for (int j = 0; j < i; j++) {
                p *= d;
            }
            powers[i][d] = p;
        }
    }
    
    // Global results set (will be merged from each thread's local results)
    set<T> global_results;
    
    // Parallelize the outer loop over digit lengths (i).
    #pragma omp parallel for schedule(dynamic)
    for (int i = 1; i <= ran; i++) {
        // Compute bounds for this digit-length.
        T upper_bound = i * powers[i][9];
        T lower_bound = (i == 1) ? 0 : 1;
        if (i > 1) {
            for (int j = 0; j < i - 1; j++) {
                lower_bound *= 10;
            }
        }
        
        // Each thread maintains its own local results.
        set<T> local_results;
        vector<int> comb;
        process_combinations(i, 0, comb, i, powers, upper_bound, lower_bound, local_results);
        
        // Merge the local results into the global results using a critical section.
        #pragma omp critical
        {
            global_results.insert(local_results.begin(), local_results.end());
        }
    }
    
    // Transfer global results into a sorted vector.
    vector<T> sorted_results(global_results.begin(), global_results.end());
    sort(sorted_results.begin(), sorted_results.end());
    return sorted_results;
}

int main() {
    int ran = 21; // Maximum number of digits to consider.

    auto start_time = chrono::high_resolution_clock::now();

    // Determine whether to use ull or __int128
    if (ran <= 18) { // Use ull for small numbers
        vector<ull> numbers = armstrong<ull>(ran);
        for (ull num : numbers) {
            cout << num << "\n";
        }
    } else { // Use __int128 for larger numbers
        vector<uint128> numbers = armstrong<uint128>(ran);
        for (uint128 num : numbers) {
            print_uint128(num);
            cout << "\n";
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "Execution time: " << elapsed.count() << " seconds\n";

    return 0;
}
