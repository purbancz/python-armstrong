#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>

using namespace std;
using ull = unsigned long long;

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
                          const vector<vector<ull>>& powers, ull upper_bound, ull lower_bound,
                          set<ull>& results) {
    if (remaining == 0) {
        // Compute candidate = sum_{d in comb} (d^length)
        ull candidate = 0;
        for (int d : comb) {
            candidate += powers[length][d];
        }
        // Discard if candidate is outside the valid range.
        if (candidate > upper_bound) return;
        if (length > 1 && candidate < lower_bound) return;
        
        // Convert candidate to its sorted digits.
        vector<int> candidate_digits;
        ull temp = candidate;
        if (temp == 0) {
            candidate_digits.push_back(0);
        } else {
            while (temp > 0) {
                candidate_digits.push_back(temp % 10);
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
vector<ull> armstrong(int ran) {
    // Precompute powers: powers[i][d] = d^i for i in 0..ran and d in 0..9.
    vector<vector<ull>> powers(ran + 1, vector<ull>(10, 0));
    for (int i = 0; i <= ran; i++) {
        for (int d = 0; d < 10; d++) {
            ull p = 1;
            for (int j = 0; j < i; j++) {
                p *= d;
            }
            powers[i][d] = p;
        }
    }
    
    set<ull> results;
    
    // For each digit-length i, generate all non-decreasing sequences of i digits.
    for (int i = 1; i <= ran; i++) {
        // Upper bound: i * (9^i)
        ull upper_bound = i * powers[i][9];
        
        // Lower bound: for i > 1, lower_bound = 10^(i-1); for i==1, no lower bound.
        ull lower_bound = (i == 1) ? 0 : 1;
        if (i > 1) {
            for (int j = 0; j < i - 1; j++) {
                lower_bound *= 10;
            }
        }
        
        vector<int> comb;
        process_combinations(i, 0, comb, i, powers, upper_bound, lower_bound, results);
    }
    
    vector<ull> sorted_results(results.begin(), results.end());
    sort(sorted_results.begin(), sorted_results.end());
    return sorted_results;
}

int main() {
    int ran = 17; // Maximum number of digits to consider.
    
    auto start_time = chrono::high_resolution_clock::now();
    vector<ull> numbers = armstrong(ran);
    auto end_time = chrono::high_resolution_clock::now();
    
    // Print Armstrong numbers.
    for (ull num : numbers) {
        cout << num << "\n";
    }
    
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "Execution time: " << elapsed.count() << " seconds\n";
    
    return 0;
}
