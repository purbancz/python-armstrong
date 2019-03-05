from itertools import combinations_with_replacement, permutations
from functools import reduce
import math
import time



start = time.time()

def numfromdig(tup):
    return reduce(lambda acc, x: acc * 10 + x, tup)

def numtodig(num):
    return tuple(map(int, str(num)))

def factors(num):
    step = 2 if num%2 else 1
    return set(reduce(list.__add__, 
                ([i, num//i] for i in range(1, int(num**0.5) + 1) if not num % i))).difference({1,num})

def iffactorssub(tup):
    return sum(tup) and factors(sum(tup)).issubset(factors(numfromdig(tup)))

def armstrong(ran):
    results = []
    for i in range(1, ran+1):

        pawlist = [pow(n, i) for n in range(10)]

        for comb in combinations_with_replacement(range(10), i):
            combval = tuple(pawlist[key] for key in comb)
            sumalist = numtodig(sum(combval))
            if len(sumalist) != i: continue
                
            for res in permutations(comb, i):
                if res == sumalist:
                    results.append(sum(combval))

    return sorted(results)

print(armstrong(7))

end = "--- %s seconds ---" % (time.time() - start)

print("Koniec")
print(end)
