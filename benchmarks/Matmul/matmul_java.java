// Written by Attractive Chaos; distributed under the MIT license

class matmul_java {
	public double[][] matgen(int n) {
		double[][] a = new double[n][n];
		double tmp = 1. / n / n;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				a[i][j] = tmp * (i - j) * (i + j);
		return a;
	}
	public double[][] matmul(double[][] a, double[][] b) {
		int m = a.length, n = a[0].length, p = b[0].length;
		double[][] x = new double[m][p];
		double[][] c = new double[p][n];
		for (int i = 0; i < n; ++i) // transpose
			for (int j = 0; j < p; ++j)
				c[j][i] = b[i][j];
		for (int i = 0; i < m; ++i)
			for (int j = 0; j < p; ++j) {
				double s = 0.0;
				for (int k = 0; k < n; ++k)
					s += a[i][k] * c[j][k];
				x[i][j] = s;
			}
		return x;
	}
	public static void main(String[] args) {
		int n = 300;
		n = n / 2 * 2;
		matmul_java m = new matmul_java();
		double[][] a, b, x;
		a = m.matgen(n);
		b = m.matgen(n);
		x = m.matmul(a, b);
		//System.out.println(x[n/2][n/2]);
	}
}
