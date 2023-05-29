static-hashmap: mkdir
	g++ src/libpack109.cpp -c -I include -std=c++11
	g++ src/liblinkedlist.cpp -c -I include -std=c++11
	g++ src/libhashmap.cpp -c -I include -std=c++11
	ar rs libhashmap.a libhashmap.o liblinkedlist.o libpack109.o
	mv *.o build/objects
	mv *.a build/lib/release

shared-hashmap: mkdir
	g++ -fPIC src/libpack109.cpp -c -I include -std=c++11
	g++ -fPIC src/liblinkedlist.cpp -c -I include -std=c++11
	g++ -fPIC src/libhashmap.cpp -c -I include -std=c++11
	g++ -shared -o libhashmap.so libhashmap.o liblinkedlist.o libpack109.o
	mv *.o build/objects
	mv *.so build/lib/release

debug-hashmap: mkdir
	g++ -fPIC src/libpack109.cpp -c -I include -std=c++11
	g++ -fPIC src/liblinkedlist.cpp -c -I include -std=c++11
	g++ -fPIC src/libhashmap.cpp -c -I include -std=c++11
	g++ -shared -g -o libhashmap.so libhashmap.o liblinkedlist.o libpack109.o
	mv *.o build/objects
	mv *.so build/lib/release

static-pack109: mkdir
	g++ src/libpack109.cpp -c -I include -std=c++11
	ar rs libpack109.a libpack109.o
	mv *.o build/objects
	mv *.a build/lib/release

shared-pack109: mkdir
	g++ -fPIC src/libpack109.cpp -c -I include -std=c++11
	g++ -shared -o libpack109.so libpack109.o
	mv *.o build/objects
	mv *.a build/lib/release

debug-pack109: mkdir
	g++ -fPIC src/libpack109.cpp -c -I include -std=c++11
	g++ -shared -g -o libpack109.so libpack109.o
	mv *.o build/objects
	mv *.a build/lib/release

mkdir:
	mkdir -p build
	mkdir -p build/lib
	mkdir -p build/bin
	mkdir -p build/objects
	mkdir -p build/lib/release
	mkdir -p build/bin/release

install: shared
	mv build/lib/release/libhashmap.so /usr/lib/libhashmap.so

clean:
	rm -rf build
	rm -f *.o
	rm -f *.a

test.o: tests/test.cpp
	g++ tests/test.cpp -c -I include

test: static test.o
	g++ test.o -o test -lhashmap -L build/lib/release
	mv test build/bin/release