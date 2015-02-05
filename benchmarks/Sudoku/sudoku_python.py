import sys, string

try:
	xrange(1)
except NameError:
	xrange = range

def sd_genmat():
	C = [[n/9, n/81*9 + n%9 + 81, n%81 + 162, n%9*9 + n/243*3 + n/27%3 + 243] for n in range(729)]
	R = [[] for c in xrange(324)]
	for r in xrange(729):
		for c2 in xrange(4):
			R[C[r][c2]].append(r)
	return R, C

def sd_update(R, C, sr, sc, r, v):
	m = 10
	m_c = 0
	for c in C[r]: sc[c] += v<<7
	for c in C[r]:
		if v > 0:
			for rr in R[c]:
				sr[rr] += 1
				if sr[rr] == 1:
					for cc in C[rr]:
						sc[cc] -= 1
						if sc[cc] < m:
							m, m_c = sc[cc], cc
		else:
			for rr in R[c]:
				sr[rr] -= 1
				if sr[rr] == 0:
					p = C[rr]
					sc[p[0]] += 1; sc[p[1]] += 1; sc[p[2]] += 1; sc[p[3]] += 1;
	return m, m_c

def sd_solve(R, C, s):
	ret, out, hints = [], [], 0
	sr = [0] * 729
	sc = [9] * 324
	cr = [-1] * 81
	cc = [-1] * 81
	for i in xrange(81):
		if ord(s[i]) >= 49 and ord(s[i]) <= 57: a = ord(s[i]) - 49
		else: a = -1
		if a >= 0:
			sd_update(R, C, sr, sc, i * 9 + a, 1)
			hints += 1
		out.append(a + 1)
	i, m, d = 0, 10, 1
	while True:
		while i >= 0 and i < 81 - hints:
			if d == 1:
				if m > 1:
					for c in xrange(324): # using enumerate() here is slower
						if sc[c] < m:
							m, cc[i] = sc[c], c
							if m < 2: break
				if m == 0 or m == 10:
					cr[i], d = -1, -1
					i -= 1
			c = cc[i]
			if d == -1 and cr[i] >= 0: sd_update(R, C, sr, sc, R[c][cr[i]], -1)
			r2_ = 9
			for r2 in xrange(cr[i] + 1, 9):
				if sr[R[c][r2]] == 0: r2_ = r2; break
			if r2_ < 9:
				m, cc[i+1] = sd_update(R, C, sr, sc, R[c][r2], 1)
				cr[i], d = r2, 1
				i += 1
			else:
				cr[i], d = -1, -1
				i -= 1
		if i < 0: break
		y = out[:81]
		for j in xrange(i):
			r = R[cc[j]][cr[j]]
			y[r//9] = r%9 + 1
		ret.append(y)
		i -= 1
		d = -1
	return ret

R, C = sd_genmat()
for line in sys.stdin:
	if len(line) >= 81:
		ret = sd_solve(R, C, line)
		#for j in ret:
		#	print(''.join(map(str, j)))
		#print('')
