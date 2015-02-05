#!/usr/bin/perl -w

# Writen by Attractive Chaos; distributed under the MIT license

use strict;
use warnings;

&main;

sub main {
	my $n = $ARGV[0] || 100;
	$n = int($n/2) * 2;
	my (@a, @b, @x);
	&matgen($n, \@a); &matgen($n, \@b);
	&mul(\@a, \@b, \@x);
	print $x[$n/2][$n/2], "\n";
}

sub transpose {
	my ($a, $b) = @_;
	my $m = @$a;
	my $n = @{$a->[0]};
	@$b = ();
	for my $i (0 .. $n - 1) {
		@{$b->[$i]} = ();
		for my $j (0 .. $m - 1) {
			push(@{$b->[$i]}, $a->[$j][$i]);
		}
	}
}
sub mul {
	my ($a, $b, $x) = @_;
	my $m = @$a;
	my $n = @{$a->[0]};
	my $p = @{$b->[0]};
	my @c;
	&transpose($b, \@c);
	for my $i (0 .. $m - 1) {
		@{$x->[$i]} = ();
		for my $j (0 .. $p - 1) {
			my $sum = 0;
			my ($ai, $cj) = ($a->[$i], $c[$j]);
			for my $k (0 .. $n - 1) {
				$sum += $ai->[$k] * $cj->[$k];
			}
			push(@{$x->[$i]}, $sum);
		}
	}
}
sub matgen {
	my ($n, $a) = @_;
	@$a = ();
	my $tmp = 1. / $n / $n;
	for my $i (0 .. $n - 1) {
		for my $j (0 .. $n - 1) {
			$a->[$i][$j] = $tmp * ($i - $j) * ($i + $j);
		}
	}
}
