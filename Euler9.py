import numpy as np
from functools import reduce
from time import perf_counter

def EratM(N):
    """
    The Sieve of Eratosthenes implemented with numpy arrays
    Returns the list of primes lower or equal than N
    """
    if N < 11:
        first_primes = ([],[],[2],[2,3],[2,3],[2,3,5],[2,3,5],[2,3,5,7],[2,3,5,7],[2,3,5,7],[2,3,5,7])
        return(first_primes[N]) 
    N_bool = np.array([True]*(N+1))
    N_bool[0] = False
    N_bool[1] = False
    N_bool[2**2::2] = False # eiminating even numbers
    N_bool[3**2::3] = False # eiminating multiples of 3
    p = 5
    while p*p <= N:
        # start from p**2
        # because all the smaller composites have factors <p
        # and are already eliminated in previous steps
        if N_bool[p]:
            N_bool[p**2::p] = False # python doesn't care if ::p goes beyond existing array
        if N_bool[p+2]:
            N_bool[(p+2)**2::(p+2)] = False
        p = p + 6 # we can move in steps of 6
    # returns indices of nonzero elements, which in this case
    # ARE the correcponding natural numbers, which were not eliminated
    # Have to use index [0], because for technical reasons it produces a 2D array
    return list(np.nonzero(N_bool)[0]) 

def IncompletePrimeFactor(N): 
    """
    Returns the list of all prime factors of N with a possible exception
    of a single factor > sqrt(N). If N is prime, it returns an empty list.
    """
    # create the list of primes lower or equal to sqrt(N)
    primes = np.array(EratM(int(np.floor(np.sqrt(N)))))
    # if N is divisible by a prime, N%p = 0
    # so when we create an array of N%p for all primes
    # it has 0 entries for factors and non-zero one for non-factors
    # recasting it as bool turns 0 into False and non-zero to True
    # after we invert them, we get True for factors and False for non-factors
    prime_mask = np.invert(np.array(N%primes, dtype = bool))
    # now we only need to apply this mask to return only prime factors
    # but since we limited ourselves to p < sqrt(N)
    # we might miss a factor > sqrt(N), like e.g. 33 = 3*11
    # so be careful when using this output
    return list(primes[prime_mask])

def FactorMultiplicity(N):
    """
    Returns two lists, the first with all the factors of N
    the second with corresponding multimlicities of each of them.
    """
    factors = IncompletePrimeFactor(N)
    if not factors: return [N],[1] # N is prime
    multiplicity = []
    # just cycle through all the factors 
    # and check how many time N is divisible by each
    for p in factors:
        m = 0
        while N%p == 0:
            m += 1
            N = N//p
        multiplicity.append(m)
    if N != 1: # the case of a single additional factor > sqrt(N)
        factors.append(N)
        multiplicity.append(1)
        
    # must recast them as python int, because
    # since we used numpy array before, their
    # type was changed to long_scalars, the default
    # int type of numpy. If we don't do this
    # we quickly get overflow when multiplying these
    # numbers
    
    factors = [int(x) for x in factors]
    return factors, multiplicity

  
def AllDivisors(N):
    """
        Generates all divisors for a number 
        as an iterating function. Example of use:
        all_factors = [u for u in AllDivisors(20)]
        produces:
        all_factors = [1, 2, 4, 5, 10, 20]
        Note that this produces an unsorted list
        e. g. for N = 12 -> [1, 2, 4, 3, 6, 12]
    """
    factors, multiplicity = FactorMultiplicity(N) # produces lists of factors with corresponding multiplicities
    fmpairs = [(f,m) for (f,m) in zip(factors,multiplicity)] # zips them into a single list of pairs
    # e. g. for N = 60 we get [(2,2), (3,1), (5,1)]
    nfactors = len(factors)
    p = [0] * nfactors # create a list filled with zeroes for all factors
    # e. g. for N = 60  p = [0, 0, 0]
    
    # This following structure runs through 
    # all possible combinations of exponents
    # For example, for N = 60 we'll get
    # [0, 0, 0] => 1
    # [1, 0, 0] => 2
    # [2, 0, 0] => 4
    # [0, 1, 0] => 3
    # [1, 1, 0] => 6
    # [2, 1, 0] => 12
    # [0, 0, 1] => 5
    # [1, 0, 1] => 10
    # [2, 0, 1] => 20
    # [0, 1, 1] => 15
    # [1, 1, 1] => 30
    # [2, 1, 1] => 60
    
    while True: # iterator starts
        # this runs through all possible combination of factors and their powers
        # reduce with the multiplication lambda multiplies them all together
        # starts with 1, since all powers are set to 0
        # f0^p0 * f1^p1 * f2^p2 ... where each exponent p can be from 0 to m, where m is the corresponding multiplicity
        yield reduce(lambda x, y: x*y, [fmpairs[x][0]**p[x] for x in range(nfactors)], 1)
        i = 0 # after yield we always start with the first exponent
        while True:
            p[i] += 1 # increase current exponent
            if p[i] <= fmpairs[i][1]: # if it is still below or equal to the multiplicity
                break # going to yield and running reduce
            p[i] = 0 # go back to exponent = 0
            i += 1 # and shift to the next factor
            if i >= nfactors: # we have increased all exponents to max
                return # iterator stops

start = perf_counter()              
p = 1000000 # given perimeter 
k = 0
if p % 2 == 0: # for a Pythagorean triple p = 2km(m+n), so p must be even
    all_factors = [u for u in AllDivisors(p//2)] 
    if len(all_factors) != 2: # len(all_factors) == 2 means p//2 is a prime and cannot be equal to km(m+n)
        # we look for a combination of km(m+n)
        # such that 
        # m > n > 1
        # GCD(m,n) = 0
        # m and n don't have equal parity, i. e. m + n is odd
        # k is any integer
        for m in all_factors:
            if m == 1: continue
            for q in all_factors: # q = m + n
                n = q - m
                # check the three conditions
                if n >= m or n < 1: continue
                if np.gcd(m,n) > 1: continue
                if q % 2 == 0: continue
                # the resulting triple m, n, k is valid and can be used to construct a Pythagorean triangle
                k = p//(2*m*q)
                ptriple = sorted([k*m*m-k*n*n, 2*k*m*n, k*m*m+k*n*n]) # once we have k, m, n, we can generate the sides a, b, and c.
                print(ptriple, 'with sum', sum(ptriple), 'equal to', p, 'and c**2', ptriple[2]*ptriple[2], '= a**2 + b**2', ptriple[0]*ptriple[0] + ptriple[1]*ptriple[1])
                print('product a*b*c', ptriple[0]*ptriple[1]*ptriple[2])

if k == 0: print('perimeter', p, 'is impossible')

end = perf_counter()
print(end - start,'sec')


  

