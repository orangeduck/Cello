/* The MIT License

   Copyright (c) 2011 by Attractive Chaos <attractor@live.co.uk>

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

// This file implements an improved algorithm of Guenter Stertenbrink's suexco.c
// (http://magictour.free.fr/suexco.txt).

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* For Sudoku, there are 9x9x9=729 possible choices (9 numbers to choose for
   each cell in a 9x9 grid), and 4x9x9=324 constraints with each constraint
   representing a set of choices that are mutually conflictive with each other.
   The 324 constraints are classified into 4 categories:

   1. row-column where each cell contains only one number
   2. box-number where each number appears only once in one 3x3 box
   3. row-number where each number appears only once in one row
   4. col-number where each number appears only once in one column

   Each category consists of 81 constraints. We number these constraints from 0
   to 323. In this program, for example, constraint 0 requires that the (0,0)
   cell contains only one number; constraint 81 requires that number 1 appears
   only once in the upper-left 3x3 box; constraint 162 requires that number 1
   appears only once in row 1; constraint 243 requires that number 1 appears
   only once in column 1.
   
   Noting that a constraint is a subset of choices, we may represent a
   constraint with a binary vector of 729 elements. Thus we have a 729x324
   binary matrix M with M(r,c)=1 indicating the constraint c involves choice r.
   Solving a Sudoku is reduced to finding a subset of choices such that no
   choices are present in the same constaint. This is equivalent to finding the
   minimal subset of choices intersecting all constraints, a minimum hitting
   set problem or a eqivalence of the exact cover problem.

   The 729x324 binary matrix is a sparse matrix, with each row containing 4
   non-zero elements and each column 9 non-zero elements. In practical
   implementation, we store the coordinate of non-zero elements instead of
   the binary matrix itself. We use a binary row vector to indicate the
   constraints that have not been used and use a column vector to keep the
   number of times a choice has been forbidden. When we set a choice, we will
   use up 4 constraints and forbid other choices in the 4 constraints. When we
   make wrong choices, we will find an unused constraint with all choices
   forbidden, in which case, we have to backtrack to make new choices. Once we
   understand what the 729x324 matrix represents, the backtracking algorithm
   itself is easy.

   A major difference between the algorithm implemented here and Guenter
   Stertenbrink's suexco.c lies in how to count the number of the available
   choices for each constraint. Suexco.c computes the count with a loop, while
   the algorithm here keeps the count in an array. The latter is a little more
   complex to implement as we have to keep the counts synchronized all the time,
   but it is 50-100% faster, depending on the input.
 */

// the sparse representation of the binary matrix
class sdaux_t {
	uint16_t r[324][9]; // M(r[c][i], c) is a non-zero element
	uint16_t c[729][4]; // M(r, c[r][j]) is a non-zero element
	
	public:
	sdaux_t();
	int update(int8_t sr[729], uint8_t sc[324], int r, int v);
	int solve(const char *_s);
	
};

// generate the sparse representation of the binary matrix
sdaux_t::sdaux_t() {
	int i, j, k, r, c, c2, r2;
	int8_t nr[324];
	for (i = r = 0; i < 9; ++i) // generate c[729][4]
		for (j = 0; j < 9; ++j)
			for (k = 0; k < 9; ++k) // this "9" means each cell has 9 possible numbers
				this->c[r][0] = 9 * i + j,                  // row-column constraint
				this->c[r][1] = (i/3*3 + j/3) * 9 + k + 81, // box-number constraint
				this->c[r][2] = 9 * i + k + 162,            // row-number constraint
				this->c[r][3] = 9 * j + k + 243,            // col-number constraint
				++r;
	for (c = 0; c < 324; ++c) nr[c] = 0;
	for (r = 0; r < 729; ++r) // generate r[][] from c[][]
		for (c2 = 0; c2 < 4; ++c2)
			k = this->c[r][c2], this->r[k][nr[k]++] = r;
}
// update the state vectors when we pick up choice r; v=1 for setting choice; v=-1 for reverting
int sdaux_t::update(int8_t sr[729], uint8_t sc[324], int r, int v)
{
	int c2, min = 10, min_c = 0;
	for (c2 = 0; c2 < 4; ++c2) sc[this->c[r][c2]] += v<<7;
	for (c2 = 0; c2 < 4; ++c2) { // update # available choices
		int r2, rr, cc2, c = this->c[r][c2];
		if (v > 0) { // move forward
			for (r2 = 0; r2 < 9; ++r2) {
				if (sr[rr = this->r[c][r2]]++ != 0) continue; // update the row status
				for (cc2 = 0; cc2 < 4; ++cc2) {
					int cc = this->c[rr][cc2];
					if (--sc[cc] < min) // update # allowed choices
						min = sc[cc], min_c = cc; // register the minimum number
				}
			}
		} else { // revert
			const uint16_t *p;
			for (r2 = 0; r2 < 9; ++r2) {
				if (--sr[rr = this->r[c][r2]] != 0) continue; // update the row status
				p = this->c[rr]; ++sc[p[0]]; ++sc[p[1]]; ++sc[p[2]]; ++sc[p[3]]; // update the count array
			}
		}
	}
	return min<<16 | min_c; // return the col that has been modified and with the minimal available choices
}
// solve a Sudoku; _s is the standard dot/number representation
int sdaux_t::solve(const char *_s)
{
	int i, j, r, c, r2, dir, cand, n = 0, min, hints = 0; // dir=1: forward; dir=-1: backtrack
	int8_t sr[729], cr[81]; // sr[r]: # times the row is forbidden by others; cr[i]: row chosen at step i
	uint8_t sc[324]; // bit 1-7: # allowed choices; bit 8: the constraint has been used or not
	int16_t cc[81]; // cc[i]: col chosen at step i
	char out[82];
	for (r = 0; r < 729; ++r) sr[r] = 0; // no row is forbidden
	for (c = 0; c < 324; ++c) sc[c] = 0<<7|9; // 9 allowed choices; no constraint has been used
	for (i = 0; i < 81; ++i) {
		int a = _s[i] >= '1' && _s[i] <= '9'? _s[i] - '1' : -1; // number from -1 to 8
		if (a >= 0) this->update(sr, sc, i * 9 + a, 1); // set the choice
		if (a >= 0) ++hints; // count the number of hints
		cr[i] = cc[i] = -1, out[i] = _s[i];
	}
	for (i = 0, dir = 1, cand = 10<<16|0, out[81] = 0;;) {
		while (i >= 0 && i < 81 - hints) { // maximum 81-hints steps
			if (dir == 1) {
				min = cand>>16, cc[i] = cand&0xffff;
				if (min > 1) {
					for (c = 0; c < 324; ++c) {
						if (sc[c] < min) {
							min = sc[c], cc[i] = c; // choose the top constraint
							if (min <= 1) break; // this is for acceleration; slower without this line
						}
					}
				}
				if (min == 0 || min == 10) cr[i--] = dir = -1; // backtrack
			}
			c = cc[i];
			if (dir == -1 && cr[i] >= 0) this->update(sr, sc, this->r[c][cr[i]], -1); // revert the choice
			for (r2 = cr[i] + 1; r2 < 9; ++r2) // search for the choice to make
				if (sr[this->r[c][r2]] == 0) break; // found if the state equals 0
			if (r2 < 9) {
				cand = this->update(sr, sc, this->r[c][r2], 1); // set the choice
				cr[i++] = r2; dir = 1; // moving forward
			} else cr[i--] = dir = -1; // backtrack
		}
		if (i < 0) break;
		for (j = 0; j < i; ++j) r = this->r[cc[j]][cr[j]], out[r/9] = r%9 + '1'; // print
		//puts(out);
		++n; --i; dir = -1; // backtrack
	}
	return n; // return the number of solutions
}

int main()
{
	sdaux_t *a = new sdaux_t();
	char buf[1024];
	while (fgets(buf, 1024, stdin) != 0) {
		if (strlen(buf) < 81) continue;
		a->solve(buf);
		//putchar('\n');
	}
  delete a;
	return 0;
}
