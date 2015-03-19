import java.util.ArrayList;
import java.util.Random;
import java.io.*;

class list_java {
	public static void main(String[] args) {
		
		ArrayList<Integer> x = new ArrayList<Integer>();
    Random rand = new Random();
		
    int n = 10000;
    
    for (int i = 0; i < n; i++) {
      x.add(rand.nextInt());
    }
    
    for (int i = 0; i < n; i++) {
      x.add(rand.nextInt(n), rand.nextInt());
    }
    
    for (int i = 0; i < n; i++) {
      x.remove(rand.nextInt(n));
    }
    
	}
}
