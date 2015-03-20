// Writen by Attractive Chaos; distributed under the MIT license

#include <stdlib.h>
#include <stdio.h>

double **mm_init(int n)
{
	double **m;
	int i;
	m = (double**)malloc(n * sizeof(void*));
	for (i = 0; i < n; ++i)
		m[i] = calloc(n, sizeof(double));
	return m;
}
void mm_destroy(int n, double **m)
{
	int i;
	for (i = 0; i < n; ++i) free(m[i]);
	free(m);
}
double **mm_gen(int n)
{
	double **m, tmp = 1. / n / n;
	int i, j;
	m = mm_init(n);
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			m[i][j] = tmp * (i - j) * (i + j);
	return m;
}
// better cache performance by transposing the second matrix
double **mm_mul(int n, double *const *a, double *const *b)
{
	int i, j, k;
	double **m, **c;
	m = mm_init(n); c = mm_init(n);
	for (i = 0; i < n; ++i) // transpose
		for (j = 0; j < n; ++j)
			c[i][j] = b[j][i];
	for (i = 0; i < n; ++i) {
		double *p = a[i], *q = m[i];
		for (j = 0; j < n; ++j) {
			double t = 0.0, *r = c[j];
			for (k = 0; k < n; ++k)
				t += p[k] * r[k];
			q[j] = t;
		}
	}
	mm_destroy(n, c);
	return m;
}
int main(int argc, char *argv[])
{
	int n = 300;
	double **a, **b, **m;
	n = (n/2) * 2;
	a = mm_gen(n); b = mm_gen(n);
	m = mm_mul(n, a, b);
	//fprintf(stderr, "%lf\n", m[n/2][n/2]);
	mm_destroy(n, a); mm_destroy(n, b); mm_destroy(n, m);
	return 0;
}

