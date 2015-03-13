rb = dofile "/home/dan/Projects/libCello/benchmarks/Map/rb.lua" 

local h = rb.new()
local max = 0
for l in io.lines() do
	local c = rb.search(h, l)
	if (c) then
	  rb.insert(h, l, c+1)
	  max = max > c and max or c
	else
	  rb.insert(h, l, 1)
  end
end

rb.delete(h)

-- print(n, max)
