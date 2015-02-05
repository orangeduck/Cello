using System;
using System.Collections.Generic;

class dict_v1 {
	static void Main() {
		string l;
		Dictionary<string, int> h = new Dictionary<string, int>();
		int max = 1;
		while ((l = Console.ReadLine()) != null) {
			int v;
			if (h.TryGetValue(l, out v)) {
				if (v+1 > max) max = v+1;
				h[l] = v+1;
			} else h[l] = 1;
		}
		Console.WriteLine(h.Count);
		Console.WriteLine(max);
	}
}
