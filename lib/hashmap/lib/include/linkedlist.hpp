#include <stdio.h>
#include <stdbool.h>
#include "llnode.hpp"
#include <stdlib.h>

namespace linkedlist{

    class LinkedList {
    private:
        Node* head;
        Node* tail;
    public:
        size_t length;
        LinkedList();
        ~LinkedList();
        size_t insertAtTail(string filename, File file);
        size_t insertAtHead(string filename, File file);
        size_t insertAtIndex(size_t index, string filename, File file);
        File remove(string filename);
        File removeHead();
        File removeTail();
        File removeAtIndex(size_t index);
        bool containsKey(string filename);
        int indexOf(string filename);
        File itemAtIndex(int index);
        bool replaceFile(string filename, File file);
        void printList();
    };
}
