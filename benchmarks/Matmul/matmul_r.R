matgen <- function(n) {
	y0 <- matrix(rep(seq(0, n-1), n), n)
	y1 <- t(y0)
	z <- 1 / n / n
	(y0 - y1) * (y0 + y1) * z
}

matmul <- function(a, b) {
	# transposing or not seems to make no difference
	m <- dim(a)[1]
	n <- dim(a)[2]
	p <- dim(b)[2]
	x <- matrix(rep(0, m * p), m)
	for (i in seq(1, m)) {
		for (j in seq(1, p)) {
			#x[i,j] <- sum(a[i,], b[,j]) # using this line: 43.3 sec, 64.6 MB
			s <- 0; ai <- a[i,]; bj <- b[,j]; for (k in seq(1, n)) s <- s + ai[k] * bj[k]; x[i,j] <- s
		}
	}
	x
}

n <- as.integer(commandArgs(trailingOnly = T)[1])
if (is.na(n)) n <- 100

# x <- matgen(n) %*% matgen(n) # using this line: 2.7 sec, 53.0 MB
x <- matmul(matgen(n), matgen(n))
x[n/2+1,n/2+1]
