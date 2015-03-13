require 'rbtree'

h, max = RBTree[], 0
STDIN.each do |l|
	if h[l] == nil
		h[l] = 1
	else
		h[l] = h[l] + 1
	end
	max = h[l] if max < h[l]
end
# puts h.length, max
