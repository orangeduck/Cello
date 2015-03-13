import random

l = []
n = 10000

random.seed(123)

for i in xrange(n):
    l.append(random.randint(0, 32767))
    
for i in xrange(n):
    l.insert(
      random.randint(0, n),
      random.randint(0, 32767))
      
for i in xrange(n):
    l.pop(random.randint(0, n))
    
del l
