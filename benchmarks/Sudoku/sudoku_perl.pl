use strict;
use warnings;

my @aux = &sd_genmat();
while (<>) {
	chomp;
	if (length($_) >= 81) {
		my $ret = &sd_solve($aux[0], $aux[1], $_);
		print($_, "\n") for (@$ret);
		print("\n");
	}
}

sub sd_genmat() {
	my (@C, @R);
	for my $i (0..8) {
		for my $j (0..8) {
			for my $k (0..8) {
				push(@C, [9*$i+$j, int($i/3)*27+int($j/3)*9+$k+81, 9*$i+$k+162, 9*$j+$k+243])
			}
		}
	}
	for my $r (0..728) {
		for my $k (@{$C[$r]}) {
			push(@{$R[$k]}, $r);
		}
	}
	return (\@R, \@C);
}

sub sd_update() {
	my ($R, $C, $sr, $sc, $r, $v) = @_;
	my ($min, $min_c) = (10, 0);
	for my $c (@{$C->[$r]})  {
		if ($v > 0) { $sc->[$c] += 128; }
		else { $sc->[$c] -= 128; }
	}
	for my $c (@{$C->[$r]}) {
		if ($v > 0) {
			for my $rr (@{$R->[$c]}) {
				++$sr->[$rr];
				next if ($sr->[$rr] != 1);
				for my $cc (@{$C->[$rr]}) {
					if (--$sc->[$cc] < $min) {
						$min = $sc->[$cc];
						$min_c = $cc;
					}
				}
			}
		} else {
			for my $rr (@{$R->[$c]}) {
				--$sr->[$rr];
				next if ($sr->[$rr] != 0);
				my $p = $C->[$rr];
				++$sc->[$p->[0]]; ++$sc->[$p->[1]]; ++$sc->[$p->[2]]; ++$sc->[$p->[3]];
			}
		}
	}
	return $min<<16 | $min_c;
}

sub sd_solve() {
	my ($R, $C, $s) = @_;
	my (@sr, @sc, @cr, @cc, @ret);
	my $hints = 0;
	for my $r (0..728) { $sr[$r] = 0; }
	for my $c (0..323) { $sc[$c] = 9; }
	for my $i (0..80) {
		my $a = substr($s, $i, 1) =~ /[1-9]/? ord(substr($s, $i, 1)) - 49 : -1;
		if ($a >= 0) {
			&sd_update($R, $C, \@sr, \@sc, $i*9+$a, 1);
			++$hints;
		}
		$cr[$i] = $cc[$i] = -1;
	}
	#for my $i (0..323) {print("$i\t$sc[$i]\n")}
	my ($i, $dir, $cand) = (0, 1, 10<<16);
	for (;;) {
		while ($i >= 0 && $i < 81 - $hints) {
			if ($dir == 1) {
				my $min = $cand>>16;
				$cc[$i] = $cand&0xffff;
				if ($min > 1) {
					for my $c (0..323) {
						if ($sc[$c] < $min) {
							$min = $sc[$c];
							$cc[$i] = $c;
							last if ($min <= 1);
						}
					}
				}
				if ($min == 0 || $min == 10) {
					$cr[$i--] = $dir = -1;
				}
			}
			my $c = $cc[$i];
			&sd_update($R, $C, \@sr, \@sc, $R->[$c][$cr[$i]], -1) if ($dir == -1 && $cr[$i] >= 0);
			my $r2;
			for ($r2 = $cr[$i] + 1; $r2 < 9; ++$r2) {
				last if ($sr[$R->[$c][$r2]] == 0);
			}
			if ($r2 < 9) {
				$cand = &sd_update($R, $C, \@sr, \@sc, $R->[$c][$r2], 1);
				$cr[$i++] = $r2; $dir = 1;
			} else {
				$cr[$i--] = $dir = -1;
			}
		}
		last if ($i < 0);
		for my $j (0..$i-1) {
			my $r = $R->[$cc[$j]][$cr[$j]];
			substr($s, int($r/9), 1) = $r%9 + 1;
		}
		push(@ret, $s);
		--$i; $dir = -1;
	}
	return \@ret;
}
