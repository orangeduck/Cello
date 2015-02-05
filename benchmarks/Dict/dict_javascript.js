

var h = {}, n = 0, max = 0;
function processLine (l) {
	if (h[l]) {
		++h[l];
		if (max < h[l]) max = h[l];
	} else h[l] = 1, ++n;
}

var readline = require('readline');
var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false
});

rl.on('line', function(line){
    processLine(line);
})

//print(n, max)
