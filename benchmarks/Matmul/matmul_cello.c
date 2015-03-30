#include "Cello.h"

struct Matrix {
  size_t n;
  double** d;
};

static void Matrix_New(var self, var args) {
  struct Matrix* m = self;
  m->n = c_int(get(args, $I(0)));
	m->d = malloc(m->n * sizeof(double*));
	for (int i = 0; i < m->n; ++i) {
		m->d[i] = calloc(m->n, sizeof(double));
	}
}

static void Matrix_Del(var self) {
	struct Matrix* m = self;
	for (int i = 0; i < m->n; ++i) free(m->d[i]);
	free(m->d);
}

static var Matrix = Cello(Matrix, Instance(New, Matrix_New, Matrix_Del));

static struct Matrix* Matrix_Gen(int n) {
	struct Matrix* m = new(Matrix, $I(n));
	double tmp = 1.0 / n / n;
	for (int i = 0; i < m->n; ++i) {
		for (int j = 0; j < m->n; ++j) {
			m->d[i][j] = tmp * (i - j) * (i + j);
		}
	}
	return m;
}

static var Matrix_Mul(var m0, var m1) {
  struct Matrix* a = m0;
  struct Matrix* b = m1;
	struct Matrix* m = new(Matrix, $I(a->n));
	struct Matrix* c = new(Matrix, $I(a->n));
	for (int i = 0; i < m->n; ++i) {
		for (int j = 0; j < m->n; ++j) {
			c->d[i][j] = b->d[j][i];
		}
	}
	for (int i = 0; i < m->n; ++i) {
		double *p = a->d[i], *q = m->d[i];
		for (int j = 0; j < m->n; ++j) {
			double t = 0.0, *r = c->d[j];
			for (int k = 0; k < m->n; ++k) {
				t += p[k] * r[k];
			}
			q[j] = t;
		}
	}
	del(c);
	return m;
}

int main(int argc, char *argv[]) {
	int n = 300;
	n = (n/2) * 2;
	var a = Matrix_Gen(n);
	var b = Matrix_Gen(n);
	var m = Matrix_Mul(a, b);
	del(a); del(b); del(m);
	return 0;
}

