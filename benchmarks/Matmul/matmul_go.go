// Written by Attractive Chaos; distributed under the MIT license

package main

import "fmt"
import "flag"
import "strconv"

func matgen(n int) [][]float64 {
	a := make([][]float64, n)
	tmp := float64(1.0) / float64(n) / float64(n) // pretty silly...
	for i := 0; i < n; i++ {
		a[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			a[i][j] = tmp * float64(i-j) * float64(i+j)
		}
	}
	return a
}

func matmul(a [][]float64, b [][]float64) [][]float64 {
	m := len(a)
	n := len(a[0])
	p := len(b[0])
	x := make([][]float64, m)
	c := make([][]float64, p)
	for i := 0; i < p; i++ {
		c[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			c[i][j] = b[j][i]
		}
	}
	for i, am := range a {
		x[i] = make([]float64, p)
		for j, cm := range c {
			s := float64(0)
			for k, m := range am {
				s += m*cm[k]
			}
			x[i][j] = s
		}
	}
	return x
}

func main() {
	n := int(100)
	flag.Parse()
	if flag.NArg() > 0 { n,_ = strconv.Atoi(flag.Arg(0)) }
	a := matgen(n)
	b := matgen(n)
	x := matmul(a, b)
	fmt.Printf("%f\n", x[n/2][n/2])
}
