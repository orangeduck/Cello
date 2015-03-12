var RBTree = require('bintrees').RBTree;

var strcmp = function (a, b) { return a.localeCompare(b); };

var h = new RBTree(strcmp), n = 0, max = 0;

function processLine (l) {
	var val = h.find(l)
    if (val) {
		h.insert(val+1);
		if (max < val+1) max = val+1;
	} else h.insert(l, 1), ++n;
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
