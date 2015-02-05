local h, max, n = {}, 0, 0
for l in io.lines() do
	if (h[l]) then h[l] = h[l] + 1
	else n, h[l] = n + 1, 1 end
	max = max > h[l] and max or h[l]
end
-- print(n, max)
