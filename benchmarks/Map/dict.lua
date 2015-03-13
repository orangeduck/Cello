
local prv = {}
local pub = {}

dict = pub

function prv.rot_less(node)
	local R = node.greater
	local M = R.less
	if R.weight == 0 then
		node.weight, R.weight = 1, -1
	else
		node.weight, R.weight = 0, 0
	end
	if M then M.parent = node end
	node.greater, node.parent, R.less, R.parent =
		M, R, node, node.parent
	return R
end

function prv.rot_greater(node)
	local L = node.less
	local M = L.greater
	if L.weight == 0 then
		node.weight, L.weight = -1, 1
	else
		node.weight, L.weight = 0, 0
	end
	if M then M.parent = node end
	node.less, node.parent, L.greater, L.parent = M, L, node, node.parent
	return L
end

function prv.rot2_less(node)
	local R = node.greater
	local M = R.less
	local ML, MR = M.less, M.greater
	if M.weight == 0 then
		node.weight, R.weight = 0, 0
	elseif M.weight == -1 then
		node.weight, R.weight, M.weight = 0, 1, 0
	else
		node.weight, R.weight, M.weight = -1, 0, 0
	end
	if ML then ML.parent = node end
	if MR then MR.parent = R end
	node.greater, node.parent, M.less, M.greater, M.parent, R.less, R.parent = 
		ML, M, node, R, node.parent, MR, M
	return M
end

function prv.rot2_greater(node)
	local L = node.less
	local M = L.greater
	local ML, MR = M.less, M.greater
	if M.weight == 0 then
		node.weight, L.weight = 0, 0
	elseif M.weight == -1 then
		node.weight, L.weight, M.weight = 1, 0, 0
	else
		node.weight, L.weight, M.weight = 0, -1, 0
	end
	if ML then ML.parent = L end
	if MR then MR.parent = node end
	node.less, node.parent, M.greater, M.less, M.parent, L.greater, L.parent =
		MR, M, node, L, node.parent, ML, M
	return M
end

function prv.rebalance(dict, node)
	local p, n, short = node.parent
	if node.weight == -2 then
		local w = node.less.weight
		if w == 1 then
			short = 1
			n = %prv.rot2_greater(node)
		else
			short = w == -1
			n = %prv.rot_greater(node)
		end
	elseif node.weight == 2 then
		local w = node.greater.weight
		if w == -1 then
			short = 1
			n = %prv.rot2_less(node)
		else
			short = w == 1
			n = %prv.rot_less(node)
		end
	elseif node.weight ~= 0 or not p then
		return
	else
		return p.less == node and 1 or -1
	end

	if not p then
		dict.root = n
	elseif node == p.less then
		p.less = n
		return short and 1
	else
		p.greater = n
		return short and -1
	end
end

local node_tag = newtag()

function prv.node(record, parent)
	local n = {
		record = record,
		weight = 0,
		parent = parent,
	}
	settag(n, %node_tag)
	return n
end

local dict_tag = newtag()
local dict_meta = {}

settagmethod(dict_tag, "index", function(_, index)
	return %dict_meta[index]
end)

function dict_meta:insert(record)
	local n, node = self.root, %prv.node(record)
	if not n then
		self.root = node
	else
		local ro = self.record_order
		local p, t
		repeat
			if n.weight ~= 0 then t = n end
			p = n
			if ro(record, n.record) < 0 then
				n = n.less
				if not n then
					node.parent = p
					n = node
					p.less = n
					break
				end
			else
				n = n.greater
				if not n then
					node.parent = p
					n = node
					p.greater = n
					break
				end
			end
		until nil
		while p ~= t do
			p.weight = n == p.less and -1 or 1
			p, n = p.parent, p
		end
		if t then
			if n == t.less then
				t.weight = t.weight - 1
			else
				t.weight = t.weight + 1
			end
			%prv.rebalance(self, t)
		end
	end	
	self.n = self.n + 1
	return node
end

function dict_meta:erase(node)
	assert(tag(node) == %node_tag, "erase takes an iterator!")
	local p, n, swap = node.parent
	if node.less then
		if node.greater then
			-- both children
			p, n = node, node.greater
			while n.less do
				p, n = n, n.less
			end
			swap, node, n = node, n, n.greater
		else
			-- only less child
			n = node.less
		end
	elseif node.greater then
		-- only greater child
		n = node.greater
	else
		-- no children
		-- n = nil
	end
	
	if n then n.parent = p end
	
	self.n = self.n - 1
	
	if not p then
		self.root = n
		return
	end
	
	if p.greater == node then
		p.greater = n
		p.weight = p.weight - 1
	else
		p.less = n
		p.weight = p.weight + 1
	end
	
	if swap then
		local sl, sg, sp = swap.less, swap.greater, swap.parent
		node.weight, node.parent, node.less, node.greater =
			swap.weight, sp, sl, sg
			
		if sl then sl.parent = node end
		if sg then sg.parent = node end
		if not sp then
			self.root = node
		elseif sp.less == swap then
			sp.less = node
		else
			sp.greater = node
		end
		
		if p == swap then p = node end
	end
	
	local w
	p, w = p.parent, %prv.rebalance(self, p)
	while w do
		p.weight = p.weight + w
		p, w = p.parent, %prv.rebalance(self, p)
	end
end

function dict_meta:find(key)
	local n, t = self.root
	local ko = self.key_order
	while n do
		local ord = ko(key, n.record)
		if ord > 0 then
			n = n.greater
		elseif ord < 0 then
			n = n.less
		else
			t, n = n, n.less
		end
	end
	if t then
		return t.record, t
	end
end

function dict_meta:lwb(key)
	local n, t = self.root
	local ko = self.key_order
	while n do
		if ko(key, n.record) > 0 then
			n = n.greater
		else
			t, n = n, n.less
		end
	end
	if t then
		return t.record, t
	end
end

function dict_meta:upb(key)
	local n, t = self.root
	local ko = self.key_order
	while n do
		if ko(key, n.record) < 0 then
			t, n = n, n.less
		else
			n = n.greater
		end
	end
	if t then
		return t.record, t
	end
end

function dict_meta:next(node)
	local n
	if node then
		assert(tag(node) == %node_tag, "next takes an iterator")
		n = node.greater
	elseif self.root then
		n = self.root
	else
		return
	end
	if n then
		repeat
			node, n = n, n.less
		until not n
		return node.record, node
	end
	
	repeat
		n, node = node, node.parent
	until not node or node.less == n
	
	if node then
		return node.record, node
	end
end

function dict_meta:prev(node)
	local n
	if node then
		assert(tag(node) == %node_tag, "prev takes an iterator")
		n = node.less
	elseif self.root then
		n = self.root
	else
		return
	end
	if n then
		repeat
			node, n = n, n.greater
		until not n
		return node.record, node
	end
	
	repeat
		n, node = node, node.parent
	until not node or node.greater == n
	
	if node then
		return node.record, node
	end
end

function dict_meta:first()
	return self:next()
end

function dict_meta:last()
	return self:prev()
end

function pub.create(record_order, key_order)
	local d = {
		record_order = record_order or %pub.cmp,
		key_order = key_order or record_order or %pub.cmp,
		n = 0,
	}
	settag(d, %dict_tag)
	return d
end

function pub.record(node)
	assert(tag(node) == %node_tag, "record takes an iterator")
	return node.record
end

function pub.cmp(a, b)
	return a < b and -1 or (b < a and 1 or 0)
end
