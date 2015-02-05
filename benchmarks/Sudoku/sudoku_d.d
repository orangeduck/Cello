// Works with LDC1
// Compile with: ldc -O3 -release -inline
// Contributed by leonardo

/*
The MIT License

Copyright (c) 2011 by Attractive Chaos <attractor@live.co.uk>
                      leonardo

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

// This implements an improved algorithm of Guenter Stertenbrink's suexco.c
// (http://magictour.free.fr/suexco.txt).

import tango.stdc.stdio: puts, fgets, stdin, putchar;
import tango.stdc.string: strlen;


/// the sparse representation of the binary matrix
struct Sdaux {
    int[9][324] r; // M(r[c][i], c) is a non-zero element
    int[4][729] c; // M(r, c[r][j]) is a non-zero element

    /// generate the sparse representation of the binary matrix
    void initialize() { // static this() is slower
        // generate this.c
        {
            int r1 = 0;
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                    for (int k = 0; k < 9; k++) { // this "9" means each cell has 9 possible numbers
                        c[r1][] = [9 * i + j, (i/3*3 + j/3) * 9 + k + 81, 9 * i + k + 162, 9 * j + k + 243];
                        r1++;
                    }
        }

        // generate this.r from this.c
        byte[r.length] nr;
        foreach (ushort r2, ref rowc; this.c)
            foreach (c2, int k; rowc) {
                this.r[k][nr[k]] = r2;
                nr[k]++;
            }
    }
}


/// update the state vectors when we pick up choice r; v=1 for setting choice; v=-1 for reverting
int sdUpdate(in Sdaux* aux, byte[Sdaux.c.length] sr,
             ubyte[Sdaux.r.length] sc, in int r, in int v) {
    int min = 10, min_c;

    for (size_t c2 = 0; c2 < aux.c[0].length; c2++)
        sc[aux.c[r][c2]] += v << 7;

    for (size_t c2 = 0; c2 < aux.c[0].length; c2++) { // update # available choices
        int c = aux.c[r][c2];
        int rr;

        if (v > 0) { // move forward
            for (size_t r2 = 0; r2 < 9; r2++) {
                if (sr[rr = aux.r[c][r2]]++ != 0)
                    continue; // update the row status
                for (size_t cc2 = 0; cc2 < aux.c[0].length; cc2++) {
                    int cc = aux.c[rr][cc2];
                    sc[cc]--;
                    if (sc[cc] < min) { // update # allowed choices
                        min = sc[cc]; // register the minimum number
                        min_c = cc;
                    }
                }
            }
        } else { // revert
            for (size_t r2 = 0; r2 < 9; r2++) {
                if (--sr[rr = aux.r[c][r2]] != 0)
                    continue; // update the row status
                auto p = aux.c[rr].ptr;
                sc[p[0]]++; // update the count array
                sc[p[1]]++;
                sc[p[2]]++;
                sc[p[3]]++;
            }
        }
    }

    return (min << 16) | min_c; // return the col that has been modified and
                                // with the minimal available choices
}


/// solve a Sudoku; _s is the standard dot/number representation
int sdSolve(in Sdaux* aux, in char* _s) {
    int hints;
    byte[Sdaux.c.length] sr; // sr[r]: # times the row is forbidden by others
    byte[81] cr = -1; // cr[i]: row chosen at step i
    ubyte[Sdaux.r.length] sc = 9; // bit 1-7: # allowed choices;
                                  // bit 8: the constraint has been used or not
                                  // 9 allowed choices; no constraint has been used
    short[cr.length] cc = -1; // cc[i]: col chosen at step i
    char[82] outs;

    for (int i = 0; i < cr.length; i++) {
        int a = (_s[i] >= '1' && _s[i] <= '9') ? (_s[i] - '1') : -1; // number from -1 to 8
        if (a >= 0)
            sdUpdate(aux, sr, sc, i * 9 + a, 1); // set the choice
        if (a >= 0)
            hints++; // count the number of hints
        outs[i] = _s[i];
    }

    int dir; // dir=1: forward; dir=-1: backtrack
    int i, r, cand, n, min;

    for (i = 0, dir = 1, cand = (10 << 16) | 0, outs[81] = 0; ; ) {
        while (i >= 0 && i < 81 - hints) { // maximum 81-hints steps
            if (dir == 1) {
                min = cand >> 16;
                cc[i] = cand & 0xFFFF;

                if (min > 1) {
                    for (size_t c = 0; c < sc.length; c++) { // performance-critical loop
                        if (sc[c] < min) {
                            min = sc[c]; // choose the top constraint
                            cc[i] = cast(short)c;
                            if (min <= 1)
                                break; // this is for acceleration; slower without this line
                        }
                    }
                }

                if (min == 0 || min == 10) {
                    dir = cr[i] = -1; // backtrack
                    i--;
                }
            }

            int c = cc[i];
            if (dir == -1 && cr[i] >= 0)
                sdUpdate(aux, sr, sc, aux.r[c][cr[i]], -1); // revert the choice

            int r2;
            for (r2 = cr[i] + 1; r2 < 9; r2++) // search for the choice to make
                if (sr[aux.r[c][r2]] == 0)
                    break; // found if the state equals 0

            if (r2 < 9) {
                cand = sdUpdate(aux, sr, sc, aux.r[c][r2], 1); // set the choice
                cr[i] = cast(byte)r2; // moving forward. r2 < 9
                i++;
                dir = 1;
            } else {
                dir = cr[i] = -1; // backtrack
                i--;
            }
        }

        if (i < 0)
            break;

        for (size_t j = 0; j < i; j++) {
            r = aux.r[cc[j]][cr[j]];
            outs[r / 9] = cast(char)(r % 9 + '1'); // print
        }

        puts(outs.ptr);
        n++; // backtrack
        i--;
        dir = -1;
    }

    return n; // return the number of solutions
}


void main() {
    Sdaux a;
    a.initialize();
    char[1024] buf;

    while (fgets(buf.ptr, buf.length, stdin) != null) {
        if (strlen(buf.ptr) < 81)
            continue;
        sdSolve(&a, buf.ptr);
        putchar('\n');
    }
}
