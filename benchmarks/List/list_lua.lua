require "math"

l = {}
n = 10000

for i=1,n do
    l[i] = math.random(0, 32767)
end

for i=1,n do
    table.insert(l, 
      math.random(1, n),
      math.random(0, 32767))
end
      
for i=1,n do
    table.remove(l, math.random(1, n))
end

