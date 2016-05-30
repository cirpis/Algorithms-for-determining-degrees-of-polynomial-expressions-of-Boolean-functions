# Return the next number after S that has the same number of ones
# in its binary representation. Produces a lexicographical ordering.
def snoob(S) :
    nxt = 0
    if S != 0:
        right_one = S & (-S)
        next_higher_one_bit = S + right_one
        right_ones_pattern = S ^ next_higher_one_bit
        right_ones_pattern = (right_ones_pattern) // right_one
        right_ones_pattern = right_ones_pattern>>2
        nxt = next_higher_one_bit | right_ones_pattern
    return nxt;

# Calculate the number of ones in the binary representation
# (the Hamming weight) of the number S. 
def hamming(S):
    count = 0
    while S != 0:
        S = S&(S-1);
        count += 1
    return count

# Return the Boolean function associated with the number S as a vector.
def binary(S, l):
    c = []
    while S > 0:
        if S % 2 == 1: 
            c.append(1)
            S -= 1
        else:
            c.append(0)
        S = S//2
    while len(c)<l:
        c.append(0)
    return c

# Implementation of the coef_n algorithm described in the paper.
def coef_n(f, n):
    c = 0
    for x in range(2**n):
        if f[x] != 0:
            if (n-hamming(x)) % 2 == 0:
                c -= 1
            else:
                c += 1
    return c

# Implementation of the less_n algorithm described in the paper.
def less_n(f, n):
    return koef_n(f,n) == 0

# Implementation of the less_k algorithm described in the paper.
def less_k(f, n, k):
    p = [0 for k in range(2**n)]
    for x in range(2**n):
        if f[x] != 0:
            t = [1]
            for i in range(1, n+1):
                if x%2 == 1:
                    a = t[::]
                    t = [0 for j in range(2**(i-1))]
                    t.extend(a)
                else:
                    t.extend([-t[j] for j in range(2**(i-1))])
            p = [p[j]+t[j] for j in range(2**n)]
    for k0 in range(k, n+1):
        S = 2**k0 - 1
        while S < 2**n:
            if p[S]!=0:
                return False
            S = snoob(S)
    return True
