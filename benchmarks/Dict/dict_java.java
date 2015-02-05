import java.util.HashMap;
import java.io.*;

class dict_java {
	public static void main(String[] args) {
		BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
		HashMap<String, Integer> h = new HashMap<String, Integer>();
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
