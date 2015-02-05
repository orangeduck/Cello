using System;

class sudoku_v1 {
	int[,] R, C;
	public void genmat() {
		R = new int[324,9];
		C = new int[729,4];
		int[] nr = new int[324];
		int i, j, k, r, c, c2;
		for (i = r = 0; i < 9; ++i) // generate c[729][4]
			for (j = 0; j < 9; ++j)
				for (k = 0; k < 9; ++k) { // this "9" means each cell has 9 possible numbers
					C[r,0] = 9 * i + j;                  // row-column constraint
					C[r,1] = (i/3*3 + j/3) * 9 + k + 81; // box-number constraint
					C[r,2] = 9 * i + k + 162;            // row-number constraint
					C[r,3] = 9 * j + k + 243;            // col-number constraint
					++r;
				}
		for (c = 0; c < 324; ++c) nr[c] = 0;
		for (r = 0; r < 729; ++r) // generate r[][] from c[][]
			for (c2 = 0; c2 < 4; ++c2) {
				k = C[r,c2]; R[k,nr[k]] = r; nr[k]++;
			}
	}
	private int sd_update(int[] sr, int[] sc, int r, int v) {
		int c2, min = 10, min_c = 0;
		for (c2 = 0; c2 < 4; ++c2) sc[C[r,c2]] += v<<7;
		for (c2 = 0; c2 < 4; ++c2) { // update # available choices
			int r2, rr, cc2, c = C[r,c2];
			if (v > 0) { // move forward
				for (r2 = 0; r2 < 9; ++r2) {
					if (sr[rr = R[c,r2]]++ != 0) continue; // update the row status
					for (cc2 = 0; cc2 < 4; ++cc2) {
						int cc = C[rr,cc2];
						if (--sc[cc] < min) { // update # allowed choices
							min = sc[cc]; min_c = cc; // register the minimum number
						}
					}
				}
			} else { // revert
				for (r2 = 0; r2 < 9; ++r2) {
					if (--sr[rr = R[c,r2]] != 0) continue; // update the row status
					++sc[C[rr,0]]; ++sc[C[rr,1]]; ++sc[C[rr,2]]; ++sc[C[rr,3]]; // update the count array
				}
			}
		}
		return min<<16 | min_c; // return the col that has been modified and with the minimal available choices
	}
	// solve a Sudoku; _s is the standard dot/number representation
	public int solve(String _s) {
		int i, j, r, c, r2, dir, cand, n = 0, min, hints = 0; // dir=1: forward; dir=-1: backtrack
		int[] sr = new int[729];
		int[] cr = new int[81];
		int[] sc = new int[324];
		int[] cc = new int[81];
		int[] o = new int[81];
		for (r = 0; r < 729; ++r) sr[r] = 0; // no row is forbidden
		for (c = 0; c < 324; ++c) sc[c] = 0<<7|9; // 9 allowed choices; no constraint has been used
		for (i = 0; i < 81; ++i) {
			int a = _s[i] >= '1' && _s[i] <= '9'? _s[i] - '1' : -1; // number from -1 to 8
			if (a >= 0) sd_update(sr, sc, i * 9 + a, 1); // set the choice
			if (a >= 0) ++hints; // count the number of hints
			cr[i] = cc[i] = -1; o[i] = a;
		}
		i = 0; dir = 1; cand = 10<<16|0;
		for (;;) {
			while (i >= 0 && i < 81 - hints) { // maximum 81-hints steps
				if (dir == 1) {
					min = cand>>16; cc[i] = cand&0xffff;
					if (min > 1) {
						for (c = 0; c < 324; ++c) {
							if (sc[c] < min) {
								min = sc[c]; cc[i] = c; // choose the top constraint
								if (min <= 1) break; // this is for acceleration; slower without this line
							}
						}
					}
					if (min == 0 || min == 10) cr[i--] = dir = -1; // backtrack
				}
				c = cc[i];
				if (dir == -1 && cr[i] >= 0) sd_update(sr, sc, R[c,cr[i]], -1); // revert the choice
				for (r2 = cr[i] + 1; r2 < 9; ++r2) // search for the choice to make
					if (sr[R[c,r2]] == 0) break; // found if the state equals 0
				if (r2 < 9) {
					cand = sd_update(sr, sc, R[c,r2], 1); // set the choice
					cr[i++] = r2; dir = 1; // moving forward
				} else cr[i--] = dir = -1; // backtrack
			}
			if (i < 0) break;
			char[] y = new char[81];
			for (j = 0; j < 81; ++j) y[i] = (char)(o[i] + '1');
			for (j = 0; j < i; ++j) { r = R[cc[j],cr[j]]; y[r/9] = (char)(r%9 + '1'); }
			Console.WriteLine(new String(y));
			++n; --i; dir = -1; // backtrack
		}
		return n;
	}
	public static void Main(String[] args) {
		sudoku_v1 a = new sudoku_v1();
		String l;
		a.genmat();
		while ((l = Console.ReadLine()) != null)
			if (l.Length >= 81) {
				a.solve(l);
				Console.WriteLine();
			}
	}
}
