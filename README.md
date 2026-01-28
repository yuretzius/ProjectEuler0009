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

All Pythagorean triples can be generated using integers $m > n \geq 1$ that cannot have equal parity and must have GCD($m$,$n$) = 1. 

Then a primitive triple $a = m^2 - n^2$, $b = 2mn$, $c = m^2 + n^2$ starts a whole family of triples $(ka,kb,kc)$, $k \in \mathbb{N}$. 

Their sum:

$$p = a + b + c = km^2 - kn^2 + 2kmn +  km^2 + kn^2 = 2km(m+n)$$

So since $1000 = 2^3 5^3$, there are not that many to create $m(m+n)$. 

Because $m + n$ must be odd, it can only be $5$, $25$, or $125$. But $1000 / 125 = 8$, meaning $km = 4$, so there is no solution $m > n$.
If $m + n = 25$, $km = 20$, leading to solution $k = 1$, $m = 20$, and $n = 5$, but then GCD($m$,$n$)$=5$. So the only possibility left is
$m + n = 5$, $km = 100$ 

It is easy to see, that the only suitable combination of $m$ and $m+n$ with $m > n$ is $4$ and $5$. 
This means $k = 25$ and so the triple is $(200,375,425)$. 

The code is designed to look for solutions for much larger arbitrary perimeters.



