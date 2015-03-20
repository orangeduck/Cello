require 'Map/redblack'

local h = redblack.newTree()
local max = 0
for l in io.lines() do
	local c = redblack.find(h, l)
	if (c) then
	  -- This Red black doesn't support value types so whatever
	  redblack.insert(h, l)
	  -- max = max > c and max or c
	else
	  redblack.insert(h, l)
  end
end

-- print(n, max)
