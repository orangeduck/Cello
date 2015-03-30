#include <stdlib.h>

class Matrix {

  public:
  size_t n;
  double** d;  

  Matrix(int n);
  ~Matrix();
  
};

Matrix::Matrix(int n) {
  n = n;
	d = (double**)malloc(n * sizeof(double*));
	for (int i = 0; i < n; ++i) {
		d[i] = (double*)calloc(n, sizeof(double));
	}
}

Matrix::~Matrix() {
	for (int i = 0; i < n; ++i) free(d[i]);
	free(d);
}

static Matrix* Matrix_Gen(int n) {
	Matrix* m = new Matrix(n);
	double tmp = 1.0 / n / n;
	for (int i = 0; i < m->n; ++i) {
		for (int j = 0; j < m->n; ++j) {
			m->d[i][j] = tmp * (i - j) * (i + j);
		}
	}
	return m;
}

static Matrix* Matrix_Mul(Matrix* m0, Matrix* m1) {
  Matrix* a = m0;
  Matrix* b = m1;
	Matrix* m = new Matrix(a->n);
	Matrix* c = new Matrix(a->n);
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
	delete c;
	return m;
}

int main(int argc, char *argv[]) {
	int n = 300;
	n = (n/2) * 2;
	Matrix* a = Matrix_Gen(n);
	Matrix* b = Matrix_Gen(n);
	Matrix* m = Matrix_Mul(a, b);
	delete a; delete b; delete m;
	return 0;
}

