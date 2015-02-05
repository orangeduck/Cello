// Written by Attractive Chaos; distributed under the MIT license

// To compile: mcs -optimize+ -out:matmul_v1.run matmul_v1.cs
// Translated from Java with reference to: http://code.wikia.com/wiki/Matrix_multiplication

using System;

class matmul_v1 {
	public double[,] matgen(int n) {
		double[,] a = new double[n,n];
		double tmp = 1.0 / n / n;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				a[i,j] = tmp * (i - j) * (i + j);
		return a;
	}
	public double[,] matmul(double[,] a, double[,] b) {
		int m = a.GetLength(0), n = a.GetLength(1), p = b.GetLength(0);
		double[,] x = new double[m,p];
		double[,] c = new double[p,n];
		for (int i = 0; i < n; ++i) // transpose
			for (int j = 0; j < p; ++j)
				c[j,i] = b[i,j];
		for (int i = 0; i < m; ++i)
			for (int j = 0; j < p; ++j) {
				double s = 0.0;
				for (int k = 0; k < n; ++k)
					s += a[i,k] * c[j,k];
				x[i,j] = s;
			}
		return x;
	}
	public static void Main(String[] args) {
		int n = 100;
		if (args.GetLength(0) >= 1) n = int.Parse(args[0]);
		n = n / 2 * 2;
		matmul_v1 m = new matmul_v1();
		double[,] a, b, x;
		a = m.matgen(n);
		b = m.matgen(n);
		x = m.matmul(a, b);
		Console.WriteLine(x[n/2,n/2]);
	}
}
