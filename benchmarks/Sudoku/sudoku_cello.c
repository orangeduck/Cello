#include "Cello.h"

struct Sudoku {
  uint16_t r[324][9];
  uint16_t c[729][4];
};

static void Sudoku_New(var self, var args) {

	struct Sudoku* a = self;
	int i, j, k, r, c, c2;
	int8_t nr[324];
	for (i = r = 0; i < 9; ++i) // generate c[729][4]
		for (j = 0; j < 9; ++j)
			for (k = 0; k < 9; ++k) // this "9" means each cell has 9 possible numbers
				a->c[r][0] = 9 * i + j,                  // row-column constraint
				a->c[r][1] = (i/3*3 + j/3) * 9 + k + 81, // box-number constraint
				a->c[r][2] = 9 * i + k + 162,            // row-number constraint
				a->c[r][3] = 9 * j + k + 243,            // col-number constraint
				++r;
	for (c = 0; c < 324; ++c) nr[c] = 0;
	for (r = 0; r < 729; ++r) // generate r[][] from c[][]
		for (c2 = 0; c2 < 4; ++c2)
			k = a->c[r][c2], a->r[k][nr[k]++] = r;

}

static var Sudoku = Cello(Sudoku, Instance(New, Sudoku_New, NULL));

static int Sudoku_Update(
  struct Sudoku* aux, int8_t sr[729], uint8_t sc[324], int r, int v) {
	
	int c2, min = 10, min_c = 0;
	for (c2 = 0; c2 < 4; ++c2) sc[aux->c[r][c2]] += v<<7;
	for (c2 = 0; c2 < 4; ++c2) { // update # available choices
		int r2, rr, cc2, c = aux->c[r][c2];
		if (v > 0) { // move forward
			for (r2 = 0; r2 < 9; ++r2) {
				if (sr[rr = aux->r[c][r2]]++ != 0) continue; // update the row status
				for (cc2 = 0; cc2 < 4; ++cc2) {
					int cc = aux->c[rr][cc2];
					if (--sc[cc] < min) // update # allowed choices
						min = sc[cc], min_c = cc; // register the minimum number
				}
			}
		} else { // revert
			const uint16_t *p;
			for (r2 = 0; r2 < 9; ++r2) {
				if (--sr[rr = aux->r[c][r2]] != 0) continue; // update the row status
				p = aux->c[rr]; ++sc[p[0]]; ++sc[p[1]]; ++sc[p[2]]; ++sc[p[3]]; // update the count array
			}
		}
	}
	return min<<16 | min_c; // return the col that has been modified and with the minimal available choices
}

// solve a Sudoku; _s is the standard dot/number representation
static int Sudoku_Solve(struct Sudoku* aux, const char *_s) {
	int i, j, r, c, r2, dir, cand, n = 0, min, hints = 0; // dir=1: forward; dir=-1: backtrack
	int8_t sr[729], cr[81]; // sr[r]: # times the row is forbidden by others; cr[i]: row chosen at step i
	uint8_t sc[324]; // bit 1-7: # allowed choices; bit 8: the constraint has been used or not
	int16_t cc[81]; // cc[i]: col chosen at step i
	char out[82];
	for (r = 0; r < 729; ++r) sr[r] = 0; // no row is forbidden
	for (c = 0; c < 324; ++c) sc[c] = 0<<7|9; // 9 allowed choices; no constraint has been used
	for (i = 0; i < 81; ++i) {
		int a = _s[i] >= '1' && _s[i] <= '9'? _s[i] - '1' : -1; // number from -1 to 8
		if (a >= 0) Sudoku_Update(aux, sr, sc, i * 9 + a, 1); // set the choice
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
			if (dir == -1 && cr[i] >= 0) Sudoku_Update(aux, sr, sc, aux->r[c][cr[i]], -1); // revert the choice
			for (r2 = cr[i] + 1; r2 < 9; ++r2) // search for the choice to make
				if (sr[aux->r[c][r2]] == 0) break; // found if the state equals 0
			if (r2 < 9) {
				cand = Sudoku_Update(aux, sr, sc, aux->r[c][r2], 1); // set the choice
				cr[i++] = r2; dir = 1; // moving forward
			} else cr[i--] = dir = -1; // backtrack
		}
		if (i < 0) break;
		for (j = 0; j < i; ++j) r = aux->r[cc[j]][cr[j]], out[r/9] = r%9 + '1'; // print
		//puts(out);
		++n; --i; dir = -1; // backtrack
	}
	return n; // return the number of solutions
}

int main(int argc, char** argv) {
	struct Sudoku* a = new(Sudoku);
	char buf[1024];
	while (fgets(buf, 1024, stdin) != 0) {
		if (strlen(buf) < 81) continue;
		Sudoku_Solve(a, buf);
		//putchar('\n');
	}
	del(a);
	return 0;
}
