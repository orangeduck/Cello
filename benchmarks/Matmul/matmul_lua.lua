-- Writen by Attractive Chaos; distributed under the MIT license

matrix = {}

function matrix.T(a)
	local m, n, x = #a, #a[1], {};
	for i = 1, n do
		x[i] = {};
		for j = 1, m do x[i][j] = a[j][i] end
	end
	return x;
end

function matrix.mul(a, b)
	assert(#a[1] == #b);
	local m, n, p, x = #a, #a[1], #b[1], {};
	local c = matrix.T(b); -- transpose for efficiency
	for i = 1, m do
		x[i] = {}
		local xi = x[i];
		for j = 1, p do
			local sum, ai, cj = 0, a[i], c[j];
			-- for luajit, caching c[j] or not makes no difference; lua is not so clever
			for k = 1, n do sum = sum + ai[k] * cj[k] end
			xi[j] = sum;
		end
	end
	return x;
end

function matgen(n)
	local a, tmp = {}, 1. / n / n;
	for i = 1, n do
		a[i] = {}
		for j = 1, n do
			a[i][j] = tmp * (i - j) * (i + j - 2) 
		end
	end
	return a;
end

local n = 300;
n = math.floor(n/2) * 2;
local a = matrix.mul(matgen(n), matgen(n));
--print(a[n/2+1][n/2+1]);
