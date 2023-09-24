# Armstrong (narcissistic) numbers generator

This Python script is designed to find [Armstrong (narcissistic) numbers](https://en.wikipedia.org/wiki/Narcissistic_number) up to a range of $10^n$, where $n$ is a parameter of the `armstrong()` function.

In essence, this script uses a multiset approach (implemented via combination with replacement).

---

### Key-features of the script that aim to reduce the search space of the problem:

1. **Precomputing**: The `powers` list precomputes the `i`th power of all digits from 0 to 9 for `i` in the range `ran+1`. This avoids having to compute these powers repeatedly for each combination.

2. **Combinations with replacement**: The `combinations_with_replacement` function from the `itertools` module generates all combinations of a certain size from the digits 0 to 9, allowing each digit to be used more than once. This significantly reduces the search space compared to generating all permutations of the digits.

3. **Checking sums early**: The code calculates the sum of the `i`th powers of the digits in a combination (`combval`) and checks if this sum is greater than `9**i * i`. If it is, it skips to the next combination. This avoids unnecessary computations for combinations that cannot possibly form an Armstrong number.

4. **Sorting and comparing tuples**: The code converts the sum of `combval` into a tuple of its digits (`sumalist`), sorts both `sumalist` and the combination, and checks if they're equal. This is a more efficient way to check if a combination forms an Armstrong number compared to generating all permutations of the combination.

5. **Data structure**: The code uses a set to store the results, which automatically takes care of duplicates and keeps the results sorted. This avoids having to check for duplicates or sort the results at the end.

6. **Multiprocessing**: The code uses Python's multiprocessing module to parallelize the computation across multiple cores, which can significantly speed up the computation for larger values.

---
Example run times of the script for a given range on a low-tier machine (Intel(R) Core(TM) i5-6400 CPU @ 2.70GHz, 32 GB RAM):
| $n$ as of $10^n$ range of search | execution time [in seconds] |
|--|--|
| 1 | 0.20598888397216797
| 3 | 0.22899746894836426
| 4 | 0.19800066947937012
| 5 | 0.17897629737854004
| 6 | 0.21700048446655273
| 7 | 0.25499892234802246
| 8 | 0.27899932861328125
| 9 | 0.3490004539489746
| 10 | 0.5720007419586182
| 11 | 0.929999828338623
| 14 | 4.891000986099243
| 16 | 13.944048166275024
| 17 | 22.939016103744507
| 19 | 60.22201991081238
| 20 | 90.91005849838257
| 21 | 139.65165328979492
| 23 | 308.0671248435974
| 24 | 446.1013534069061
| 25 | 646.2790954113007
| 27 | 1464.0623307228088
<!-- | 29 | ???
| 31 | ??? ~3h?
| 32 | ???
| 33 | ???
| 34 | ???
| 35 | ??? 
| 37 | ???
| 38 | ???
| 39 | ??? -->