#include <vector>
#include <stdio.h>
#include <iostream>
#include "pack109.hpp"

using namespace std;

void write(vec serialized, char* filename) {
  FILE * pFile;

  //open file
  pFile = fopen(filename, "wb");

  if (pFile == NULL) {
    perror("Error opening file");
  } else {
    //write each byte in vector to file
    for(int i = 0; i < serialized.size(); i++) {
      fputc(serialized[i], pFile);
    }

    //close file
    fclose(pFile);
  }

  return;
}

vec read(char* filename) {
  FILE * pFile;
  vec myPerson;
  int i = 0;
  char c = 0;

  //open file
  pFile = fopen("person.pack109", "rb");

  if (pFile == NULL) {
    perror("Error opening file");
  } else {
    //for each char/byte, add it into the array
    while(c != EOF) {
      c = fgetc(pFile);
      myPerson.push_back(c);
    }

    //close file
    fclose(pFile);
  }
  return myPerson;
}

void printStruct(struct Person person) {
  int totalSize = sizeof(struct Person);
  unsigned char* personBytes = (unsigned char*)(&person);

  for(int i = 0; i < totalSize; i++) {
    printf("%x ", personBytes[i]);
  }
  printf("\n");
  cout << "Age: " << (int)person.age << "\t" << "Height: " << person.height << "\t" << "Name: " << person.name << endl;
}

int main() {
  //serialize person into byte vector
  struct Person josie = {20, 5.7, "Josie"};
  vec serialized = pack109::serialize(josie);

  //write byte vector to binary file
  write(serialized, "person.pack109");
  //read file back in as byte vector
  vec fromFile = read("person.pack109");

  //deserialize read data back into person struct
  struct Person josieDeser = pack109::deserialize_person(fromFile);

  printf("Original struct:\n");
  printStruct(josie);
  printf("Recreated Struct from File:\n");
  printStruct(josieDeser);

  return 0;
}
