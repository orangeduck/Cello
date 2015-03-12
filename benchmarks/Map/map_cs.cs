using System;
using System.Collections.Generic;

class dict_v1 {
	static void Main() {
		string l;
		SortedSet<string, int> m = new SortedSet<string, int>();
		int max = 1;
		while ((l = Console.ReadLine()) != null) {
			int v;
			if (m.TryGetValue(l, out v)) {
				if (v+1 > max) max = v+1;
				m[l] = v+1;
			} else m[l] = 1;
		}
		Console.WriteLine(m.Count);
		Console.WriteLine(max);
	}
}
