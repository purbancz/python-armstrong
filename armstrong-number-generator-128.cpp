#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <omp.h>
#include <string>
#include <sstream>

using namespace std;
using uint128 = unsigned __int128;

// Helper: Convert a uint128 value to its decimal string representation.
string uint128ToString(uint128 x) {
    if (x == 0) return "0";
    string s;
    while (x > 0) {
        int digit = x % 10;
        s.push_back('0' + digit);
        x /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

/**
 * Recursively generate and process combinations (with replacement) of digits.
 *
 * @param remaining   Number of digits left to choose.
 * @param start       Current starting digit (to ensure non-decreasing order).
 * @param comb        Current combination of digits.
 * @param length      Total length of the combination.
 * @param powers      Precomputed table where powers[i][d] = d^i.
 * @param upper_bound Upper bound for a valid candidate: i * (9^i).
 * @param lower_bound Lower bound for a valid candidate (10^(i-1) for i>1, or 0 for i==1).
 * @param results     Set to store valid Armstrong numbers.
 */
void process_combinations(int remaining, int start, vector<int>& comb, int length,
                          const vector<vector<uint128>>& powers, uint128 upper_bound, uint128 lower_bound,
                          set<uint128>& results) {
    if (remaining == 0) {
        // Compute candidate = sum_{d in comb} (d^length)
        uint128 candidate = 0;
        for (int d : comb) {
            candidate += powers[length][d];
        }
        // Discard if candidate is outside the valid range.
        if (candidate > upper_bound) return;
        if (length > 1 && candidate < lower_bound) return;
        
        // Convert candidate to its sorted digits.
        vector<int> candidate_digits;
        uint128 temp = candidate;
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
 *
 * @param ran Maximum number of digits to consider.
 * @return    Sorted vector of Armstrong numbers.
 */
vector<uint128> armstrong(int ran) {
    // Precompute powers: powers[i][d] = d^i for i in 0..ran and d in 0..9.
    vector<vector<uint128>> powers(ran + 1, vector<uint128>(10, 0));
    for (int i = 0; i <= ran; i++) {
        for (int d = 0; d < 10; d++) {
            uint128 p = 1;
            for (int j = 0; j < i; j++) {
                p *= d;
            }
            powers[i][d] = p;
        }
    }
    
    // Global results set (will be merged from each thread's local results)
    set<uint128> global_results;
    
    // Parallelize the outer loop over digit lengths (i).
    #pragma omp parallel for schedule(dynamic)
    for (int i = 1; i <= ran; i++) {
        // Compute bounds for this digit-length.
        uint128 upper_bound = i * powers[i][9];
        uint128 lower_bound = (i == 1) ? 0 : 1;
        if (i > 1) {
            for (int j = 0; j < i - 1; j++) {
                lower_bound *= 10;
            }
        }
        
        // Each thread maintains its own local results.
        set<uint128> local_results;
        vector<int> comb;
        process_combinations(i, 0, comb, i, powers, upper_bound, lower_bound, local_results);
        
        // Merge the local results into the global results using a critical section.
        #pragma omp critical
        {
            global_results.insert(local_results.begin(), local_results.end());
        }
    }
    
    // Transfer global results into a sorted vector.
    vector<uint128> sorted_results(global_results.begin(), global_results.end());
    sort(sorted_results.begin(), sorted_results.end());
    return sorted_results;
}

int main() {
    int ran = 21; // Maximum number of digits to consider.
    
    auto start_time = chrono::high_resolution_clock::now();
    vector<uint128> numbers = armstrong(ran);
    auto end_time = chrono::high_resolution_clock::now();
    
    // Print Armstrong numbers.
    for (auto num : numbers) {
        cout << uint128ToString(num) << "\n";
    }
    
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "Execution time: " << elapsed.count() << " seconds\n";
    
    return 0;
}
