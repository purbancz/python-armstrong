from itertools import combinations_with_replacement
from multiprocessing import Pool


def check_comb(comb, i, powers):
    combval = [powers[i][n] for n in comb]
    sum_combval = sum(combval)
    max_sum = 9**i * i

    if sum_combval > max_sum:
        return None

    sumalist = tuple(map(int, str(sum_combval)))

    # Sort sumalist and the combination and check if they're equal
    if tuple(sorted(comb)) == tuple(sorted(sumalist)):
        return sum_combval

    return None

def armstrong(ran):
    # Precompute the ith power of all digits from 0 to 9
    powers = [[n**i for n in range(10)] for i in range(ran+1)]
    
    results = set()

    with Pool() as pool:
        for i in range(1, ran+1):
            # Use a pool of workers to check the combinations in parallel
            for result in pool.starmap(check_comb, [(comb, i, powers) for comb in combinations_with_replacement(range(10), i)]):
                if result is not None:
                    results.add(result)

    return sorted(list(results))

# Run the program to find Armstrong numbers with n digits
if __name__ == '__main__':
    import time
    start = time.time()
    print(armstrong(14))
    end = time.time()
    print(f"Execution time: {end - start} seconds")