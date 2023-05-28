#include "hashmap.hpp"
#include <stdio.h>


int main() {
	hashmap::HashMap hs = hashmap::HashMap();
  string s = "file.txt";
  File f = {name: s, bytes: {0x01, 0x02, 0x03}};
  hs.insert(s, f);
  hs.print();
  printf("\n");
  f.bytes = {0x01, 0x02, 0x03, 0x04};
  hs.insert(s, f);
  hs.print();
  printf("\n");
  hs.remove(s);
  hs.insert(s, f);
  hs.print();
  printf("\n");
  vec byte = hs.get("ssss").bytes;
  pack109::printVec(byte);
  vec byte2 = hs.get("file.txt").bytes;
  pack109::printVec(byte2);
  
  return 0;
}
