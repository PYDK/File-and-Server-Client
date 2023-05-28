#include "hashmap.hpp"

unsigned long hashmap::HashMap::prehash(string filename) {
    int s = sizeof(filename);
    int k = 0;
    unsigned long h = 5381;
    char* myptr = (char*)&filename[0];
    while (k < s) {
        char* c = (char*)(myptr + k);
        h = ((h << 5) + h) + *c;
        k++;
    }
    return h;
}

hashmap::HashMap::HashMap(size_t size) {
    this->array = new linkedlist::LinkedList*[size];
    for (int i = 0;i < size;i++) {
        this->array[i] = new linkedlist::LinkedList();
    }
    this->size = size;
}

hashmap::HashMap::HashMap() {
    this->array = new linkedlist::LinkedList * [10];
    for (int i = 0;i < 10;i++) {
        this->array[i] = new linkedlist::LinkedList();
    }
    this->size = 10;
}

hashmap::HashMap::~HashMap() {
    for (int i = 0;i < this->size;i++) {
        array[i]->~LinkedList();
    }
}

unsigned long hashmap::HashMap::hash(string filename) {
    unsigned long pre = this->prehash(filename);
    unsigned long s = (unsigned long)(pre % this->size);
    return s;
}


bool hashmap::HashMap::insert(string filename, File file) {
    unsigned long hashed = hash(filename);
    if (this->contains(filename) == true) {
        int index = this->array[hashed]->replaceFile(filename, file);
        return true;
    }

    this->array[hashed]->insertAtTail(filename, file);
    int buckets = 0;
    for (int i = 0;i < this->size;i++) {
        if (this->array[i]->length > 0) {
            buckets++;
        }
    }
    double loadfactor = (double)(buckets) / (double)(this->size);
    if (loadfactor >= 0.70) {
        this->resize(this->size * 2);
    }
    return false;
}

File hashmap::HashMap::get(string filename) {
    unsigned long hashed = hash(filename);
    if (this->contains(filename) == true) {
        int index = this->array[hashed]->indexOf(filename);
        return this->array[hashed]->itemAtIndex(index);
    }
    else {
        printf("File not found: \"%s\"\n", filename.c_str());
        return (File){name: "", bytes: {}};
    }
}


bool hashmap::HashMap::remove(string filename) {
    if (this->contains(filename) == false) {
        return false;
    }
    unsigned long hashed = hash(filename);
    File f = this->array[hashed]->remove(filename);
    if (f.name.compare(filename) == 0) {
        return true;
    }
    return false;
}


bool hashmap::HashMap::contains(string filename) {
    unsigned long hashed = hash(filename);
    bool res = this->array[hashed]->containsKey(filename);
    return res;
}


void hashmap::HashMap::resize(size_t new_size) {
    linkedlist::LinkedList** old = this->array;
    File element[this->len()];
    size_t limit = this->size;
    size_t currentindex = 0;
    for (size_t i = 0;i < limit;i++) {
        if (this->array[i]->length > 0) {
            for (int j = 0;j < this->array[i]->length;j++) {
                element[currentindex++] = this->array[i]->itemAtIndex(j);
            }
        }
    }
    this->size = new_size;
    this->array = new linkedlist::LinkedList * [new_size];
    for (int i = 0;i < new_size;i++) {
        this->array[i] = new linkedlist::LinkedList();
    }
    for (int i = 0;i < currentindex;i++) {
        this->insert(element[i].name, element[i]);
    }
    for (int i = 0;i < limit;i++) {
        old[i]->~LinkedList();
    }
}


size_t hashmap::HashMap::len() {
    size_t len = 0;
    for (int i = 0;i < this->size;i++) {
        len += this->array[i]->length;
    }
    return len;
}



size_t hashmap::HashMap::capacity() {
    return (size_t)(0.7 * (double)this->size) - 1;
}


void hashmap::HashMap::print() {
    for (int i = 0;i < this->size;i++) {
        printf("List %d: ", i + 1);
        this->array[i]->printList();
        printf("\n");
    }
}