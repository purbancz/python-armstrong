# mailto: p.m.urbanczyk@gmail.com
from itertools import combinations_with_replacement
from multiprocessing import Pool
from numba import njit


@njit
def check_comb(comb, i, powers, max_sum):
    sum_combval = sum(powers[i][n] for n in comb)

    if sum_combval > max_sum:
        return None

    sumalist = tuple(sorted(map(int, str(sum_combval))))
    
    if tuple(sorted(comb)) == sumalist:
        return sum_combval

    return None

def armstrong(ran):
    powers = [[n**i for n in range(10)] for i in range(ran+1)]
    max_sums = [9**i * i for i in range(ran+1)]
    results = set()

    with Pool() as pool:
        for i in range(1, ran+1):
            combinations_gen = combinations_with_replacement(range(10), i)
            for result in pool.starmap(check_comb, ((comb, i, powers, max_sums[i]) for comb in combinations_gen), chunksize=1000):
                if result is not None:
                    results.add(result)

    return sorted(results)

# Run the program to find Armstrong numbers with n digits
if __name__ == '__main__':
    import time
    start = time.time()
    print(armstrong(14))
    end = time.time()
    print(f"Execution time: {end - start} seconds")