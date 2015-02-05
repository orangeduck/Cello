function sd_genmat()
	local R, C, r = {}, {}, 0
	for i = 0, 8 do
		for j = 0, 8 do
			for k = 0, 8 do
				C[r], r = { 9 * i + j, math.floor(i/3)*27 + math.floor(j/3)*9 + k + 81,
							9 * i + k + 162, 9 * j + k + 243 }, r + 1
			end
		end
	end
	for c = 0, 323 do R[c] = {} end
	for r = 0, 728 do
		for c2 = 1, 4 do table.insert(R[C[r][c2]], r) end
	end
	return R, C
end

function sd_update(R, C, sr, sc, r, v)
	local min, min_c = 10, 0
	for c2 = 1, 4 do
		if v > 0 then sc[C[r][c2]] = sc[C[r][c2]] + 128
		else sc[C[r][c2]] = sc[C[r][c2]] - 128 end
	end
	for c2 = 1, 4 do
		local c = C[r][c2]
		if v > 0 then
			for r2 = 1, 9 do
				local rr = R[c][r2]
				sr[rr] = sr[rr] + 1
				if sr[rr] == 1 then
					for cc2 = 1, 4 do
						local cc = C[rr][cc2]
						sc[cc] = sc[cc] - 1
						if sc[cc] < min then min, min_c = sc[cc], cc end
					end
				end
			end
		else
			for r2 = 1, 9 do
				local rr = R[c][r2]
				sr[rr] = sr[rr] - 1
				if sr[rr] == 0 then
					local p = C[rr]
					sc[p[1]], sc[p[2]], sc[p[3]], sc[p[4]] = sc[p[1]]+1, sc[p[2]]+1, sc[p[3]]+1, sc[p[4]]+1
				end
			end
		end
	end
	return min, min_c
end

function sd_solve(R, C, s)
	local sr, sc, cr, cc, hints = {}, {}, {}, {}, 0
	for r = 0, 728 do sr[r] = 0 end
	for c = 0, 323 do sc[c] = 9 end
	for i = 0, 80 do
		local t = s:byte(i+1)
		local a = t >= 49 and t <= 57 and t - 49 or -1
		if a >= 0 then sd_update(R, C, sr, sc, i * 9 + a, 1); hints = hints + 1 end
		cr[i], cc[i] = 0, 0
	end
	local i, min, dir, ret = 0, 10, 1, {}
	while true do
		while i >= 0 and i < 81 - hints do
			if dir == 1 then
				if min > 1 then
					for c = 0, 323 do
						if sc[c] < min then
							min, cc[i] = sc[c], c
							if min < 2 then break end
						end
					end
				end
				if min == 0 or min == 10 then cr[i], dir, i = 0, -1, i - 1 end
			end
			local c, r2_ = cc[i], 10
			if dir == -1 and cr[i] > 0 then sd_update(R, C, sr, sc, R[c][cr[i]], -1) end
			for r2 = cr[i] + 1, 9 do
				if sr[R[c][r2]] == 0 then r2_ = r2; break end
			end
			if r2_ < 10 then
				min, cc[i+1] = sd_update(R, C, sr, sc, R[c][r2_], 1)
				cr[i], dir, i = r2_, 1, i + 1
			else cr[i], dir, i = 0, -1, i - 1 end
		end
		if i < 0 then break end
		local y = {}
		for j = 1, 81 do y[j] = s:byte(j) - 48 end
		for j = 0, i - 1 do
			r = R[cc[j]][cr[j]]
			y[math.floor(r/9)+1] = math.fmod(r, 9) + 1
		end
		ret[#ret+1] = y
		dir, i = -1, i - 1
	end
	return ret
end

local R, C = sd_genmat()
for l in io.lines() do
	if #l >= 81 then
		local ret = sd_solve(R, C, l)
		--for _, v in ipairs(ret) do print(table.concat(v)) end
		--print()
	end
end
