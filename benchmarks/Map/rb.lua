---------------------------------------
--
--  rb.lua 
--  20110212
--
--  from cormen 2nd edition
--
---------------------------------------

--[[
node = {
  k;    -- key
  red;  -- true or false
  l;    -- left subtree
  r;    -- right subtree
  p;    -- parent
  data; -- for the client
}

tree = {
  root;
  NIL;  -- sentinel node: parent of root / leaves
}
--]]

-- used for the simetric operations
-- d is either 'left'/'right' or true/false (or error !)
-- returns 'l','r' or 'r','l'
local function direction(d)
  if d == 'left' or d == true then
    return 'l','r','left','right'
  elseif d == 'right' or d == false then
    return 'r','l','right','left'
  else
    error("bad argument to 'direction' : either 'left' or 'right'")
  end
end

local function new()
  local NIL = {}
  NIL.p = NIL
  NIL.l = NIL
  NIL.r = NIL
  return {root = NIL, NIL=NIL}
end

-- names of variables are consistent with minimum
local function edge(x, NIL, d)
  local L = direction(d)
  while x[L] ~= NIL do
    x = x[L]
  end
  return x
end

-- names of variables are consistent with successor (!)
local function next(x, NIL, d)
  local _, R = direction(d)
  if x[R] ~= NIL then
    return edge(x[R], NIL, d)
  end
  local y = x.p
  while y ~= NIL and x == y[R] do
    x = y
    y = y.p
  end
  return y
end

local function search(x, k, NIL)
  while x ~= NIL and x.k ~= k do
    if k < x.k then
      x = x.l
    else
      x = x.r
    end
  end
  return x
end

-- names of variables are consistent with left rotation
local function rotate (t,x,d)
  local L,R = direction(d)
  local y = x[R]
  x[R] = y[L]
  if y[L] ~= t.NIL
    then y[L].p = x end
  y.p = x.p
  if x.p == t.NIL 
    then t.root = y
    else if x == x.p[L] 
            then x.p[L] = y
            else x.p[R] = y end end
  y[L] = x
  x.p = y
end

local function fixinsert(t,z)
  local n = z
  while z.p.red do  
    local L,R,LEFT,RIGHT = direction(z.p == z.p.p.l)
    local y = z.p.p[R]
    if y.red then 
      z.p.red = nil
      y.red = nil
      z.p.p.red = true
      z = z.p.p
    else 
      if z == z.p[R] then 
        z = z.p
        rotate(t,z,LEFT)
      end
      z.p.red = nil
      z.p.p.red = true
      rotate(t,z.p.p,RIGHT)
    end
  end
  t.root.red = nil
  return n
end

local function insert(t,k,data)
  local NIL = t.NIL
  local y = NIL
  local x = t.root
  
  while x ~= NIL 
    do y = x
    if k == x.k then return x end
    if k < x.k 
      then x = x.l
      else x = x.r end end
  local z = {k=k,data=data}
  z.p = y
  if y == NIL 
    then t.root = z
    else 
      if k == y.k then return y end
      if k < y.k 
         then y.l = z
         else y.r = z end end
  z.l = NIL
  z.r = NIL
  z.red = true
  z.data = data
  return fixinsert(t,z)
end

local function fixdelete(t, x)
  local y,z,w
  while x ~= t.root and not x.red do
    local L,R,LEFT,RIGHT = direction(x == x.p.l)
    w = x.p[R]
    if w.red then
      w.red = nil
      x.p.red = true
      rotate(t, x.p, LEFT)
      w = x.p[R]
    end
    if not w[L].red and not w[R].red then 
      w.red = true
      x = x.p
    else
      if not w[R].red then
        w[L].red = nil
        w.red = true
        rotate(t, w, RIGHT)
        w = x.p[R]
      end
      w.red = x.p.red
      x.p.red = nil
      w[R].red = nil
      rotate(t, x.p, LEFT)
      x = t.root
    end
  end
  x.red = nil
end

local function delete(t, z)
  local NIL = t.NIL
  local x,y
  if z.l == NIL or z.r == NIL 
     then y = z 
     else y = next(z, NIL, 'left') end
  if y.l ~= NIL 
     then x = y.l 
     else x = y.r end
  x.p = y.p 
  if y.p == NIL 
     then t.root = x 
     else if y == y.p.l 
             then y.p.l = x
             else y.p.r = x end end
  if y ~= z 
     then z.k = y.k
          z.data = y.data end
  if not y.red
     then fixdelete(t, x) end
  return y
end

return {
  new         = new;
  minimum     = function(x,NIL) return edge(x,NIL,'left') end;
  maximum     = function(x,NIL) return edge(x,NIL,'right') end;
  predecessor = function(x,NIL) return next(x,NIL,'right') end;
  successor   = function(x,NIL) return next(x,NIL,'left') end;
  search      = search;
  insert      = insert;
  delete      = delete;
}
