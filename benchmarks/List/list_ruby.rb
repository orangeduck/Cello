
l = []
n = 10000

n.times {
  l.push(rand())
}

n.times {
  l.insert(rand(n), rand())
}
      
n.times {
  l.delete_at(rand(n))
}
