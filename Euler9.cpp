#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

/* this is basically Python script rewritten with C++ vectors etc.*/


//the sieve of Eratosthenes, the limit by both memory and time is about 10^10
// older version with step 2
// (10^10 takes about 137 seconds)
vector<unsigned long> Erat(unsigned long N){
    vector<unsigned long> primes;
    if (N == 0 || N == 1) return primes;
    primes.push_back(2);
    if (N == 2) return primes;
    primes.push_back(3);
    if (N == 3) return primes;
    vector<bool> Nbool(N+1, true);
    vector<bool>::iterator itr = Nbool.begin();
    *itr = false;
    *(itr+1) = false;
    //*(itr+2) = true;
    //*(itr+3) = true;
    for (itr = Nbool.begin() + 4; itr < Nbool.end(); itr += 2) *itr = false;
    unsigned long p = 3;
    while (p*p <= N) {
        for (itr = Nbool.begin() + p*p; itr < Nbool.end(); itr += p) *itr = false;
        p += 2;
        while (! Nbool[p]) p += 2;
    }
    for (itr = Nbool.begin()+5; itr < Nbool.end(); itr += 2) {
        if (*itr) primes.push_back(itr - Nbool.begin());
    }
    return primes;
}

//the sieve of Eratosthenes, the limit by both memory and time is about 10^10
// newer version with step 6
// (10^10 takes about 132 seconds)
vector<unsigned long> EratM(unsigned long N){
    vector<unsigned long> primes;
    if (N < 11) {
        if (N < 2) return primes;
        primes.push_back(2);
        if (N == 2) return primes;
        primes.push_back(3);
        if (N < 5) return primes;
        primes.push_back(5);
        if (N < 7) return primes;
        primes.push_back(7);
        return primes;
    }
    vector<bool> Nbool(N+1, true);
    vector<bool>::iterator itr = Nbool.begin();
    *itr = false;
    *(itr+1) = false;
    for (itr = Nbool.begin() + 4; itr < Nbool.end(); itr += 2) *itr = false;
    for (itr = Nbool.begin() + 9; itr < Nbool.end(); itr += 3) *itr = false;
    unsigned long p = 5;
    while (p*p <= N) {
        if (Nbool[p]) {
            for (itr = Nbool.begin() + p*p; itr < Nbool.end(); itr += p) *itr = false;
        }
        if (Nbool[p+2]) {
            for (itr = Nbool.begin() + (p+2)*(p+2); itr < Nbool.end(); itr += p+2) *itr = false;
        }
        p += 6;
    }
    primes.push_back(2);
    for (itr = Nbool.begin()+3; itr < Nbool.end(); itr += 2) {
        if (*itr) primes.push_back(itr - Nbool.begin());
    }
    return primes;
}


vector<unsigned long> IncompletePrimeFactor(unsigned long N) {
    vector<unsigned long> primes = EratM((unsigned long)(floor(sqrt((double) N))));
    vector<unsigned long>::iterator itr;
    vector<unsigned long> factors;
    for (itr = primes.begin(); itr != primes.end(); itr ++) {
        if (N % *itr == 0) factors.push_back(*itr);
    }
    return factors;
} 
  
vector<pair <unsigned long,int>> FactorMultiplicity(unsigned long N){
    vector<unsigned long> factors = IncompletePrimeFactor(N);
    vector<pair <unsigned long,int>> mfactors;
    if (factors.empty()) {
        mfactors.push_back(make_pair(N,1));
        return mfactors;
    }
    vector<unsigned long>::iterator itr;
    int m;
    for (itr = factors.begin(); itr != factors.end(); itr++) {
        m = 0;
        while (N % *itr == 0) {
            m++;
            N /= *itr;
        }
        mfactors.push_back(make_pair(*itr,m));
    }
    if (N != 1) mfactors.push_back(make_pair(N, 1));
    return mfactors;
}

int ipow(int base, int exp) 
// power function for integers
// works as follows:
// say we have base = 2 and exp = 5 or 101 in binary
// cycle 1: last bit is 1, base now is 2, so result becomes 2, bitshift to 10, base becomes 4
// cycle 2: last bit 0, so no change to result, shift 10 to 1, base becomes 16
// cycle 3: last bit 1, base is now 16, so result becomes 32, shift to 0, cycle terminates, returns 32
{
    int result = 1;
    for (;;) {
        if (exp & 1) // checks the last bit of exp is 1, equivalent to exp % 2
            result *= base;
        exp >>= 1; // bit shift by one bit to the right, equivalent to exp / 2
        if (!exp) // done
            break;
        base *= base;
    }
    return result;
}

unsigned long collapse(vector<pair <unsigned long,int>> mfactors, vector<int> f) 
// this is just the equivalent of python's 
// reduce(lambda x, y: x*y, [fmpairs[x][0]**p[x] for x in range(nfactors)], 1)
// i. e. take factors from the first items of pairs and exponentiate them to the given powers
{
    vector<pair <unsigned long,int>>::iterator itrmf;
    vector<int>::iterator itrf;
    unsigned long prod = 1;
    for (itrmf = mfactors.begin(), itrf = f.begin(); (itrmf != mfactors.end()) && (itrf != f.end()); itrmf++, itrf++) {
        if (*itrf != 0) prod *= ipow((*itrmf).first, *itrf);
    }
    return prod;
}

vector <unsigned long> AllDivisors(unsigned long N) {
    vector<pair <unsigned long,int>> mfactors = FactorMultiplicity(N);
    vector <unsigned long> propfactors;
    int nf = mfactors.size();
    vector<int> f(nf, 0);
    bool run = true;
    while (run) {
        propfactors.push_back(collapse(mfactors,f));
        int i = 0;
        while (true) {
            f[i] += 1;
            if (f[i] <= mfactors[i].second) break;
            f[i] = 0;
            i++;
            if (i == nf) {
                run = false;
                break;
            }
        }
    }
    return propfactors;
}

int main() {
    cout << "enter perimeter (for 10 000 000 a*b*c already overflows ulong):" << endl;
    unsigned long p;
    cin >> p;
    clock_t c_start = clock();
    int k = 0;
    unsigned long maxprod, prod;
    maxprod = 0;
    if (p % 2 == 0) {
    vector <unsigned long> factors = AllDivisors(p/2);
        if (factors.size() != 2) {
            vector <unsigned long>::iterator mm,q;
            for (mm = factors.begin(); mm != factors.end(); mm++) {
                unsigned long m = *mm;
                if (m == 1) continue;
                for (q = factors.begin(); q != factors.end(); q++) {
                    unsigned long n = *q - m;
                    if ((n >= m) || (n < 1)) continue;
                    if ((m + n) % 2 == 0) continue;
                    if (__gcd(n,m) > 1) continue;
                    k = p/(2*m**q);
                    unsigned long a = k*m*m-k*n*n;
                    unsigned long b = 2*k*n*m;
                    unsigned long c = k*m*m+k*n*n;
                    prod = a*b*c;
                    if (prod > maxprod) maxprod = prod;
                    if (a > b) swap(a,b);
                    cout << a << "\t" << b << "\t" << c << "\t" << c+b+a << "\t" << c*a*b << endl;
                } 
            }
        }
    }
    clock_t c_end = clock();
    cout << 1000.0*(c_end - c_start) / CLOCKS_PER_SEC << " ms\n";
    cout << (c_end - c_start) / CLOCKS_PER_SEC << " sec\n";
    
    if (k == 0) cout << "this perimeter is impossible!" << endl;
    //if (maxprod == 0) cout << "-1" << endl;
    else cout << "\tmaximum product: \t\t\t\t" << maxprod << endl;
    cout << "\tbeware of max unsigned long: \t\t\t" << ULONG_MAX << endl;
}

