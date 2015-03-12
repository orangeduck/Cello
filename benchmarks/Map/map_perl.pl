my (%h, $max);

while (<>) {
    next if (++$h{$_} > $max);
    $max = $h{$_};
}

print scalar(keys(%h)), "\t$max\n";
