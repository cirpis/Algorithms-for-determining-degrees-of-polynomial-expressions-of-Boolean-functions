#include <vector>

typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned char u_char;

// Return the next number after S that has the same number of ones
// in its binary representation. Produces a lexicographical ordering.
u_long snoob(u_long S) {
	u_long right_one, next_higher_one_bit, right_ones_pattern, next = 0;
	if (S) {
		right_one = S & -(signed)S;
		next_higher_one_bit = S + right_one;
		right_ones_pattern = S ^ next_higher_one_bit;
		right_ones_pattern = (right_ones_pattern) / right_one;
		right_ones_pattern >>= 2;
		next = next_higher_one_bit | right_ones_pattern;
	}
	return next;
}

// Perform the permutation that swaps the n-th input variable with the (n-1)-th input variable
// of the n-variable Boolean function S, and return the new function.
u_long permute_1(u_long S, u_char n) {
	u_short deg = 1 << (n - 2);
	u_int pot = 1 << deg;
	u_long S_p;
	S_p = S%pot;
	S_p += ((S >> deg) % pot) << (2 * deg);
	S_p += ((S >> (2 * deg)) % pot) << deg;
	S_p += ((S >> (3 * deg)) % pot) << (3 * deg);
	return S_p;
}

// Perform the permutation (12...n) on the input variables
// of the n-variable Boolean function S, and return the new function.
u_long permute_2(u_long S, u_char n) {
	u_long S_p, part_1, part_2;
	S_p = 0;
	part_1 = S % (1 << (1 << (n - 1)));
	part_2 = S >> (1 << (n - 1));
	for (u_char deg = 0; deg<(1 << (n - 1)); deg++) {
		S_p += (part_1 % 2 + (part_2 % 2) * 2) << (2 * deg);
		part_1 = part_1 >> 1;
		part_2 = part_2 >> 1;
	}
	return S_p;
}

// Perform a flip (an xor with 1) of the i-th input variable
// of the n-variable Boolean function S, and return the new function.
u_long flip(u_long S, u_char n, u_char i) {
	u_char deg = 1 << (i - 1);
	u_long pot = 1 << deg, S_p = 0, piece;
	for (u_char j = 0; j <= 1 << (n - i); j++) {
		piece = (S%pot) << deg;;
		S = S >> deg;
		piece += S%pot;
		S = S >> deg;
		S_p += piece << (deg * 2 * j);
	}
	return S_p;
}

// Find the function gained by inverting the given function S
u_long invert(u_long S, u_char n) {
	u_long S_p = S ^ ((1 << (1 << n)) - 1);
	return S_p;
}

// To each n-variable Boolean function S with an equal number of ones assign a unique index
u_long make_index(u_long S) {
    u_long index = 0;
    double fac = 1;
    u_char n=0, k = 1;
    while(S != 0) {
        if(S % 2 == 1) {
            if (!(k==1&&n==0))
                index += fac;
            fac = (fac*(n+1))/(k+1);
            k++;
            n++;
        }
        else {   
			if(n+1!=k)
                fac = (fac*(n+1))/(n-k+1);
            else
                fac = 1;
            n++;
        }
        S = S>>1;
    }
	return index;
}

// Find all functions equivalent via permutation and negation of variables to the
// of the n-variable Boolean S and return them in the vector A_i.
void make_class(u_long S, u_long index, u_char n, vector<bool> & A_i) {
	A_i[index] = 1;
	u_long nodes[4];
	nodes[0] = permute_2(S, n);
	nodes[1] = permute_1(S, n);
	nodes[2] = flip(S, n, n);
	for (u_char j = 0; j<3; j++) {
        index = make_index(nodes[j]);
        if (A_i[index] == 0)
            make_class(nodes[j], index, n, A_i);
	}
}

// Find all equivalence classes via permutation and negation of variables
// of n-variable Boolean functions and return the first member of each in vector B_i.
vector<u_long> find_all_classes(u_char n)
{
	vector<u_long> B_i;
	u_char deg = 1 << n;
	u_long S, pot = 1 << deg, d = 1, index;
	double size = n;
	vector<bool> A_i;
	B_i.push_back(0);
	for (u_char i = 1; i <= (1 << (n - 1)); i++) {
		S = (1 << i) - 1;
		A_i.resize(size+2, 0);
		index = 0;
		while (index<size) {
			if (A_i[index] == 0) {
				make_class(S, index, n, A_i);
				outfile << S << "\n";
			}
			S = snoob(S);
			index++;
		}
		A_i.clear();
        size = (size*(deg-i))/(i+1);
	}
	return B_i;
}

int main() {
	u_char n = 5;
	vector<u_long> B_i;
	B_i = find_all_classes(n);
	return 0;
}
