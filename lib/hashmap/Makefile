static:
	mkdir -p build
	mkdir -p build/lib
	mkdir -p build/lib/release
	mkdir -p build/objects
	make -C lib
	g++ src/lib.cpp -c -I include -I lib/include -I ../pack109/include
	ar rs libhashmap.a lib.o
	mv *.o build/objects
	mv *.a build/lib/release

shared:
	mkdir -p build
	mkdir -p build/lib
	mkdir -p build/lib/release
	mkdir -p build/objects
	make shared -C lib
	g++ -fPIC src/lib.cpp -c -I include -I lib/include -I ../pack109/include
	g++ -shared -o libhashmap.so lib.o
	mv *.o build/objects
	mv *.so build/lib/release

debug:
	mkdir -p build
	mkdir -p build/lib
	mkdir -p build/lib/release
	mkdir -p build/objects
	make -C lib
	g++ -g -fPIC src/lib.cpp -c -I include -I lib/include -I ../pack109/include
	g++ -shared -o libhashmap.so lib.o
	mv *.o build/objects
	mv *.so build/lib/release
	
install: shared
	mv build/lib/release/libhashmap.so /usr/lib/libhashmap.so

clean:
	make clean -C lib
	rm -rf build

test.o: tests/test.cpp
	g++ tests/test.cpp -c -I include -I lib/include -I ../pack109/include

libhashmap.a:
	mkdir -p build
	mkdir -p build/lib
	mkdir -p build/lib/release
	mkdir -p build/objects
	g++ src/lib.cpp -c -I include -I lib/include -I ../pack109/include
	ar rs libhashmap.a lib.o

test: test.o libhashmap.a
	g++ test.o -o test -lhashmap -llinkedlist -L. -L lib/build/lib/release -lpack109 -L../pack109/build/lib/release
	mkdir -p build/bin/release
	mkdir -p build/lib/release
	mkdir -p build/objects
	mv *.o build/objects
	mv *.a build/lib/release
	mv test build/bin/release
