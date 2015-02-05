def sd_genmat()
	mr = Array.new(324) { [] }
	mc = Array.new(729) { [] }
	r = 0
	(0...9).each do |i|
		(0...9).each do |j|
			(0...9).each do |k|
				mc[r] = [ 9 * i + j, (i/3*3 + j/3) * 9 + k + 81, 9 * i + k + 162, 9 * j + k + 243 ]
				r += 1
			end
		end
	end
	(0...729).each do |r|
		(0...4).each do |c2|
			mr[mc[r][c2]].push(r)
		end
	end
	return mr, mc
end

def sd_update(mr, mc, sr, sc, r, v)
	min, min_c = 10, 0
	(0...4).each do |c2|
		if v > 0 then sc[mc[r][c2]] += 128
		else sc[mc[r][c2]] -= 128 end
	end
	(0...4).each do |c2|
		c = mc[r][c2]
		if v > 0 then
			(0...9).each do |r2|
				rr = mr[c][r2]
				sr[rr] += + 1
				if sr[rr] == 1 then
					p = mc[rr]
					sc[p[0]] -= 1; sc[p[1]] -= 1; sc[p[2]] -= 1; sc[p[3]] -= 1
					if sc[p[0]] < min then min, min_c = sc[p[0]], p[0] end
					if sc[p[1]] < min then min, min_c = sc[p[1]], p[1] end
					if sc[p[2]] < min then min, min_c = sc[p[2]], p[2] end
					if sc[p[3]] < min then min, min_c = sc[p[3]], p[3] end
				end
			end
		else
			(0...9).each do |r2|
				rr = mr[c][r2]
				sr[rr] -= 1
				if sr[rr] == 0 then
					p = mc[rr]
					sc[p[0]] += 1; sc[p[1]] += 1; sc[p[2]] += 1; sc[p[3]] += 1
				end
			end
		end
	end
	return min, min_c
end

def sd_solve(mr, mc, s)
	ret, sr, sc, hints = [], Array.new(729) { 0 }, Array.new(324) { 9 }, 0
	(0...81).each do |i|
		a = (s[i].chr >= '1' and s[i].chr <= '9')? s[i].ord - 49 : -1
		if a >= 0 then sd_update(mr, mc, sr, sc, i * 9 + a, 1); hints += 1 end
	end
	cr, cc = Array.new(81) { -1 }, Array.new(81) { 0 }
	i, min, dir = 0, 10, 1
	loop do
		while i >= 0 and i < 81 - hints do
			if dir == 1 then
				if min > 1 then
					(0...324).each do |c|
						if sc[c] < min then
							min, cc[i] = sc[c], c
							if min < 2 then break end
						end
					end
				end
				if min == 0 or min == 10 then cr[i], dir, i = -1, -1, i - 1 end
			end
			c = cc[i]
			if dir == -1 and cr[i] >= 0 then sd_update(mr, mc, sr, sc, mr[c][cr[i]], -1) end
			r2_ = 9
			(cr[i]+1...9).each do |r2|
				if sr[mr[c][r2]] == 0 then r2_ = r2; break end
			end
			if r2_ < 9 then
				min, cc[i+1] = sd_update(mr, mc, sr, sc, mr[c][r2_], 1)
				cr[i], dir, i = r2_, 1, i + 1
			else cr[i], dir, i = -1, -1, i - 1 end
		end
		if i < 0 then break end
		o = []
		(0...81).each do |j| o.push(s[j].ord - 49) end
		(0...i).each do |j|
			r = mr[cc[j]][cr[j]]
			o[r/9] = r % 9 + 1
		end
		ret.push(o)
		i, dir = i - 1, -1
	end
	return ret
end

mr, mc = sd_genmat()
STDIN.each do |line|
	if line.length >= 81 then
		ret = sd_solve(mr, mc, line)
		#ret.each do |s| puts s.join end
		#puts
	end
end
