#!/usr/bin/env python3
"""
Armstrong (Narcissistic) Number Generator
==========================================
This script generates n-digit Armstrong (narcissistic) numbers.
Candidate numbers are generated via combinations_with_replacement
and then validated by comparing their digit frequencies.

Author: Piotr Urbańczyk p.m.urbanczyk@gmail.com
"""

from itertools import combinations_with_replacement
from multiprocessing import Pool
import time


def check_comb(comb, i, powers, upper_bound, lower_bound):
    sum_combval = sum(powers[i][n] for n in comb)

    if sum_combval > upper_bound:
        return None

    if i > 1 and sum_combval < lower_bound:
        return None

    sumalist = tuple(sorted(map(int, str(sum_combval))))

    if tuple(sorted(comb)) == sumalist:
        return sum_combval

    return None


def check_comb_wrapper(args):
    return check_comb(*args)


def armstrong(ran):
    powers = [[n ** i for n in range(10)] for i in range(ran + 1)]
    upper_bounds = [9 ** i * i for i in range(ran + 1)]
    lower_bounds = [0 if i == 1 else 10 ** (i - 1) for i in range(ran + 1)]

    results = set()

    with Pool() as pool:
        for i in range(1, ran + 1):
            combinations_gen = combinations_with_replacement(range(10), i)
            args_iter = ((comb, i, powers, upper_bounds[i], lower_bounds[i])
                         for comb in combinations_gen)
            for result in pool.imap_unordered(check_comb_wrapper, args_iter, chunksize=1000):
                if result is not None:
                    results.add(result)

    return sorted(results)


# Run the program to find Armstrong numbers with n digits
def main():
    start = time.time()
    numbers = armstrong(14)
    print(numbers)
    end = time.time()
    print(f"Execution time: {end - start:.2f} seconds")


if __name__ == '__main__':
    main()
