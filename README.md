# ProjectEuler0009
My work on

problem #9 of projecteuler.net:

Special Pythagorean Triplet

A Pythagorean triplet is a set of three natural numbers, $a \lt b \lt c$, for which,

$$a^2 + b^2 = c^2.$$

For example, $3^2 + 4^2 = 9 + 16 = 25 = 5^2$.

There exists exactly one Pythagorean triplet for which $a + b + c = 1000$. Find the product $abc$.

Completed on Sat, 20 Feb 2021, 22:48

#######################################################################################
Please note: Project Euler's policy allows publication of solutions for the first 100 problems,
that's why I am sharing my work here for reference and educational purposes.
#######################################################################################

The problem itself with this perimeter is very easy and does not require much computation.

All Pythagorean triples can be generated using integers $m$ and $n$ satisfying the folowing: 

1. $m > n \geq 1$ ;
2. cannot have equal parity, i. e. $m + n$ must be odd;
3. GCD ( $m$ , $n$ ) = 1. 

Then a primitive triple $a = m^2 - n^2$, $b = 2mn$, $c = m^2 + n^2$ starts a whole family of triples $(ka,kb,kc)$, $k \in \mathbb{N}$. 

Their sum:

$$p = a + b + c = km^2 - kn^2 + 2kmn +  km^2 + kn^2 = 2km(m+n)$$

So since $1000 = 2^3 5^3$, there are not that many options to create $m(m+n)$. 

Because $m + n$ must be odd, it can only be $5$, $25$, or $125$. But $1000 / 125 = 8$, meaning $km = 4$, so there is no solution $m > n$.
If $m + n = 25$, $km = 20$, leading to solution $k = 1$, $m = 20$, and $n = 5$, but then GCD($m$,$n$)$=5$. So the only possibility left is
$m + n = 5$, $km = 100$, leadning to $k = 25$, $m = 4$, and $n = 1$, the only possible solution.

The code is designed to look for solutions for much larger arbitrary perimeters. It was an excuse to work more on the prime factorization functions.
There are detailed comments in the code itself. C++ version mostly tries to emulate the python version, using C++ vectors instead of numpy arrays
and python lists, so read the python code comments first.

C++ versions of Erat(N) and EratM(N) work well even with $N = 10^{10}$, finishing in about 130 sec each. Python versions can only run up to about $N = 10^9$ in 70 sec.

