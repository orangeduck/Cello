import sys

def count_duplicates(lines):
    h, m = {}, 0
    for l in lines:
        if (l in h): h[l] += 1
        else: h[l] = 1
        if (m < h[l]): m = h[l]
    return h, m

h, m = count_duplicates(sys.stdin)
# print(len(h), m)
