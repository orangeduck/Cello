#include <stdlib.h>
#include <vector>

int main(int argc, char** argv) {
  
  std::vector<int> x = std::vector<int>();
  
  int n = 10000;
  
  for (int i = 0; i < n; i++) {
    x.push_back(rand());
  }
  
  for (int i = 0; i < n; i++) {
    x.insert(x.begin()+(rand() % n), rand());
  }
  
  for (int i = 0; i < n; i++) {
    x.erase(x.begin()+(rand() % n));
  }
  
  return 0;
  
}
