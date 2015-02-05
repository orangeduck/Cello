package main

import (
	"bufio"
	"fmt"
	"os"
)

type sdaux_t struct {
	r [324][9]uint16
	c [729][4]uint16
}

func sd_genmat() *sdaux_t {
	a := new(sdaux_t)
	nr := make([]int8, 324)
	r := 0
	for i := 0; i < 9; i++ {
		for j := 0; j < 9; j++ {
			for k := 0; k < 9; k++ {
				a.c[r][0] = uint16(9*i + j)
				a.c[r][1] = uint16((i/3*3+j/3)*9 + k + 81)
				a.c[r][2] = uint16(9*i + k + 162)
				a.c[r][3] = uint16(9*j + k + 243)
				r++
			}
		}
	}
	for c := 0; c < 324; c++ {
		nr[c] = 0
	}
	for r := 0; r < 729; r++ {
		for c2 := 0; c2 < 4; c2++ {
			k := a.c[r][c2]
			a.r[k][nr[k]] = uint16(r)
			nr[k]++
		}
	}
	return a
}

func sd_update_forward(aux *sdaux_t, sr []int8, sc []uint8, r uint16) int {
	min, min_c := uint8(10), uint16(0)
	rows := &aux.c[r]
	for _, c := range rows {
		sc[c] |= 0x80
	}
	for _, c := range rows {
		for _, rr := range &aux.r[c] {
			sr[rr]++
			if sr[rr] != 1 {
				continue
			}
			for _, cc := range &aux.c[rr] {
				v := sc[cc] - 1
				sc[cc] = v
				if v < min {
					min, min_c = v, cc
				}
			}
		}
	}
	return int(min)<<16 | int(min_c)
}

func sd_update_revert(aux *sdaux_t, sr []int8, sc []uint8, r uint16) {
	rows := &aux.c[r]
	for _, c := range rows {
		sc[c] &= 0x7f
	}
	for _, c := range rows {
		for _, rr := range &aux.r[c] {
			sr[rr]--
			if sr[rr] != 0 {
				continue
			}
			for _, cc := range &aux.c[rr] {
				sc[cc]++
			} // unroll this loop makes no difference
		}
	}
}

func sd_solve(aux *sdaux_t, _s []byte) int {
	sr := make([]int8, 729)
	cr := make([]int8, 81)
	sc := make([]uint8, 324)
	cc := make([]int16, 81)
	out := make([]byte, 81)
	n, hints := 0, 0
	for c := 0; c < 324; c++ {
		sc[c] = 9
	}
	for i := 0; i < 81; i++ {
		a := -1
		if _s[i] >= '1' && _s[i] <= '9' {
			a = int(_s[i] - '1')
		}
		if a >= 0 {
			sd_update_forward(aux, sr, sc, uint16(i*9+a))
			hints++
		}
		cr[i], cc[i], out[i] = -1, -1, _s[i]
	}
	i, dir, cand := 0, 1, 10<<16
	for {
		for i >= 0 && i < 81-hints {
			if dir == 1 {
				min := uint8(cand >> 16)
				cc[i] = int16(cand & 0xffff)
				if min > 1 {
					for c, scc := range sc {
						if scc < min {
							min, cc[i] = scc, int16(c)
							if min <= 1 {
								break
							}
						}
					}
				}
				if min == 0 || min == 10 {
					cr[i], dir = -1, -1
					i--
				}
			}
			c := cc[i]
			if dir == -1 && cr[i] >= 0 {
				sd_update_revert(aux, sr, sc, aux.r[c][cr[i]])
			}
			r2_ := 9
			for r2 := cr[i] + 1; r2 < 9; r2++ {
				if sr[aux.r[c][r2]] == 0 {
					r2_ = int(r2)
					break
				}
			}
			if r2_ < 9 {
				cand = sd_update_forward(aux, sr, sc, aux.r[c][r2_])
				cr[i], dir = int8(r2_), 1
				i++
			} else {
				cr[i], dir = -1, -1
				i--
			}
		}
		if i < 0 {
			break
		}
		for j := 0; j < i; j++ {
			r := aux.r[cc[j]][cr[j]]
			out[r/9] = byte(r%9) + '1'
		}
		fmt.Println(string(out))
		n++
		i--
		dir = -1
	}
	return n
}

func main() {
	a := sd_genmat()
	r := bufio.NewReader(os.Stdin)
	for {
		l, e := r.ReadSlice('\n')
		if e != nil {
			break
		}
		if len(l) > 81 {
			sd_solve(a, l)
			fmt.Println("")
		}
	}
}
