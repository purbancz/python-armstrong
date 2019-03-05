from itertools import combinations_with_replacement, permutations
import time


start = time.time()


def armstrong(ran):
    results = []
    for i in range(1, ran+1):

        pawlist = [n**i for n in range(10)]

        for comb in combinations_with_replacement(range(10), i):
            combval = tuple(pawlist[key] for key in comb)
            sumalist = tuple(map(int, str(sum(combval))))
            if len(sumalist) != i: continue
            
            for res in permutations(comb, i):
                if res == sumalist:
                    results.append(sum(combval))

    return sorted(results)

print(armstrong(7))

end = "--- %s seconds ---" % (time.time() - start)

print("END")
print(end)
