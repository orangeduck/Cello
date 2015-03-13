
var l = []
var n = 10000

for (var i = 0; i < n; i++) {
  l.push(Math.floor((Math.random() * 32767) + 0))
}

for (var i = 0; i < n; i++) {
  l.splice(
    Math.floor((Math.random() * n) + 0), 0,
    Math.floor((Math.random() * 32767) + 0))
}
      
for (var i = 0; i < n; i++) {
  l.pop(Math.floor((Math.random() * n) + 0))
}
