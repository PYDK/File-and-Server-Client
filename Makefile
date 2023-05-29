all: mkdir client server

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
	make static-pack109 -C lib/

hashmap:
	make static-hashmap -C lib/

client.o: mkdir
	g++ src/bin/client.cpp -c -I lib/include -std=c++11
	mv client.o build/objects/release

client: pack109 client.o
	g++ build/objects/release/client.o -o client -lpack109 -Llib/build/lib/release -std=c++11
	mv client build/bin/release

server.o: mkdir
	g++ src/bin/server.cpp -c -Ilib/include -std=c++11
	mv server.o build/objects/release

server: hashmap server.o
	g++ build/objects/release/server.o -o server -lhashmap -Llib/build/lib/release -std=c++11
	mv server build/bin/release

pack109.so:
	make shared-pack109 -C lib/

hashmap.so:
	make shared-hashmap -C lib/

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
	make clean -C lib/
	rm -f *.a
	rm -f *.o
	rm -rf build
	rm -rf received