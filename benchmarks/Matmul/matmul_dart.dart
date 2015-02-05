import 'dart:scalarlist';

mat_transpose(a)
{
	int m = a.length, n = a[0].length; // m rows and n cols
	var b = new List(n);
	for (int j = 0; j < n; ++j) b[j] = new Float64List(m);
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			b[j][i] = a[i][j];
	return b;
}

mat_mul(a, b)
{
	inner_loop(t, n, ai, c)
	{
		var xi = new Float64List(t);
		for (int j = 0; j < t; ++j) {
			double sum = 0.0;
			for (int k = 0; k < n; ++k) sum += ai[k] * c[j][k];
			xi[j] = sum;
		}
		return xi;
	}

	int m = a.length, n = a[0].length, s = b.length, t = b[0].length;
	if (n != s) return null;
	var x = new List(m), c = mat_transpose(b);
	for (int i = 0; i < m; ++i)
		x[i] = inner_loop(t, n, a[i], c);
	return x;
}

mat_gen(int n)
{
	var a = new List(n);
	double t = 1.0 / n / n;
	for (int i = 0; i < n; ++i) {
		a[i] = new Float64List(n);
		for (int j = 0; j < n; ++j)
			a[i][j] = t * (i - j) * (i + j);
	}
	return a;
}

main()
{
	List<String> argv = new Options().arguments;
	int n = argv.length != 0? int.parse(argv[0]) : 1000;
	var a = mat_gen(n), b = mat_gen(n);
	var c = mat_mul(a, b);
	print(c[n~/2][n~/2]);
}
