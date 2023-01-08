all: mkdir pack109 hashmap client server

mkdir:
	mkdir -p build
	mkdir -p build/bin
	mkdir -p build/bin/release
	mkdir -p build/objects
	mkdir -p build/objects/release
	mkdir -p build/lib
	mkdir -p build/lib/release
	mkdir -p received

pack109:
	make libpack109.a -C lib/pack109

hashmap:
	make static -C lib/hashmap

client.o:
	g++ src/bin/client.cpp -c -lpack109 -Llib/pack109/build/lib/release -Ilib/pack109/include -std=c++11
	mv client.o build/objects/release

client: client.o
	g++ build/objects/release/client.o -o client -lpack109 -Llib/pack109/build/lib/release -Ilib/pack109/include -std=c++11
	mv client build/bin/release

server.o:
	g++ src/bin/server.cpp -c -Ilib/pack109/include -Ilib/hashmap/include -Ilib/hashmap/lib/include -std=c++11
	mv server.o build/objects/release

server: server.o
	g++ build/objects/release/server.o -o server -lpack109 -Llib/pack109/build/lib/release -lhashmap -Llib/hashmap/build/lib/release -llinkedlist -Llib/hashmap/lib/build/lib/release -std=c++11
	mv server build/bin/release

pack109.so:
	make libpack109.so -C lib/pack109

hashmap.so:
	make shared -C lib/hashmap

install: mkdir pack109.so hashmap.so client server
	sudo cp lib/pack109/build/lib/release/libpack109.so /usr/lib/
	sudo cp lib/hashmap/build/lib/release/libhashmap.so /usr/lib/
	sudo cp lib/hashmap/lib/build/lib/release/liblinkedlist.so /usr/lib
	sudo cp build/bin/release/client /usr/bin/
	sudo cp build/bin/release/server /usr/bin/

uninstall:
	sudo rm /usr/lib/libpack109.so
	sudo rm /usr/lib/libhashmap.so
	sudo rm /usr/lib/liblinkedlist.so
	sudo rm /usr/bin/client
	sudo rm /usr/bin/server

clean:
	make clean -C lib/pack109
	make clean -C lib/hashmap
	rm -f *.a
	rm -f *.o
	rm -rf build
	rm -rf received