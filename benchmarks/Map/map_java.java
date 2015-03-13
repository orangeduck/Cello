import java.util.TreeMap;
import java.io.*;

class map_java {
	public static void main(String[] args) {
		BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
		TreeMap<String, Integer> h = new TreeMap<String, Integer>();
		String l;
		int max = 0;
		try {
			while ((l = stdin.readLine()) != null) {
				int x = 1;
				if (h.containsKey(l)) {
					x = h.get(l) + 1;
					h.put(l, x);
					if (x > max) max = x;
				} else h.put(l, 1);
			}
		} catch (IOException e) {
		}
		//System.out.println(h.size()+"\t"+max);
	}
}
