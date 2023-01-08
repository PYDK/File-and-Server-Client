#include "hashmap.hpp"
#include <stdbool.h>
#include "pack109.hpp"


int main() {
	hashmap::HashMap hs = hashmap::HashMap();
  string s = "file.txt";
  File f = {name: s, bytes: {0x01, 0x02, 0x03}};
  hs.insert(s, f);
  hs.print();
  f.bytes = {0x01, 0x02, 0x03, 0x04};
  hs.insert(s, f);
  hs.print();
  hs.remove(s);
  hs.insert(s, f);
  hs.print();
  vec byte = hs.get("ssss").bytes;
  pack109::printVec(byte);
  
  return 0;
}
