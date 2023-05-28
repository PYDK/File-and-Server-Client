#include "linkedlist.hpp"

    linkedlist::Node::Node() {
        this->filename = "";
        this->file = { name: "", bytes : {} };
        this->next = NULL;
    }

    linkedlist::Node::Node(string filename, File file) {
        this->filename = filename;
        this->file = (File){file.name, file.bytes};
        this->next = NULL;
    }

    linkedlist::LinkedList::LinkedList() {
        head = new Node();
        tail = new Node();
        this->head = NULL;
        this->tail = NULL;
        this->length = 0;
    }

    linkedlist::LinkedList::~LinkedList() {
        Node* temp = this->head;
        while (temp != NULL) {
            Node* t = temp;
            temp = temp->next;
            free(t);
        }
    }

    size_t linkedlist::LinkedList::insertAtHead(string filename, File file) {
        Node* node = new Node(filename, file);
        if (node == NULL) {
            return 1;
        }
        //if list is empty.
        if (this->head == NULL) {
            this->head = node;
            this->tail = node;
        } else {
            node->next = this->head;
            this->head = node;
        }
        this->length++;
        return 0;
    }


    size_t linkedlist::LinkedList::insertAtTail(string filename, File file) {
        Node* node = new Node(filename, file);
        if (node == NULL) {
            return 1;
        }
        //if list is empty.
        if (this->head == NULL) {
            this->head = node;
            this->tail = node;
        } else {
            this->tail->next = node;
            this->tail = this->tail->next;
        }
        this->length++;
        return 0;
    }

    bool linkedlist::LinkedList::containsKey(string filename) {
        Node* current = this->head;
        for (int i = 0; i < this->length;i++) {
            if (strcmp(current->filename.c_str(), filename.c_str()) == 0) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    int linkedlist::LinkedList::indexOf(string filename) {
        Node* current = this->head;
        for (int i = 0;i < this->length;i++) {
            if (strcmp(current->filename.c_str(), filename.c_str()) == 0) {
                return i;
            }
            current = current->next;
        }
        return -1;
    }

    bool linkedlist::LinkedList::replaceFile(string filename, File file) {
        Node* current = this->head;
        for (int i = 0;i < this->length;i++) {
            if (strcmp(current->filename.c_str(), filename.c_str()) == 0) {
                current->file = (File){name:file.name, bytes :file.bytes};
                return true;
            }
            current = current->next;
        }
        return false;
    }

    size_t linkedlist::LinkedList::insertAtIndex(size_t index, string filename, File file) {
        if (index == 0) {
            return insertAtHead(filename, file);
        }
        Node* current = this->head;
        Node* items = new Node(filename, file);
        if (items == NULL) {
            return 1;
        }
        for (int i = 0;i < index - 1;i++) {
            current = current->next;
        }
        items->next = current->next;
        current->next = items;
        return 0;
    }


    File linkedlist::LinkedList::removeTail() {
        Node* temp;
        int i = 0;
        File item = {};
        if (this->tail == NULL) {
            // List is Empty	
            return item;
        } else {
            temp = this->head;

            // Iterate to the end of the list
            while (temp->next != this->tail) {
                temp = temp->next;
            }

            item = this->tail->file;

            Node* old_tail = this->tail;
            this->tail = temp;
            this->tail->next = NULL;
            free(old_tail);
        }
        this->length--;
        return item;
    }


    File linkedlist::LinkedList::removeHead() {
        File item = {};
        if (this->head == NULL) {
            // List is Empty	
            return item;
        } else {
            item = this->head->file;
            Node* old_head = this->head;
            this->head = this->head->next;
            free(old_head);
        }
        this->length--;
        return item;
    }

    File linkedlist::LinkedList::remove(string filename) {
        int index = -1;
        File temp;
        Node* current = this->head;
        for (int i = 0;i < this->length;i++) {
            index++;
            if (current->filename == filename) {
                temp = this->removeAtIndex(index);

                return temp;
            }
            current = current->next;
            if (current == NULL) {
                break;
            }
        }
        return temp;
    }


    File linkedlist::LinkedList::removeAtIndex(size_t index) {
        File returnVal = {};
        if (index < 0 || index >= this->length) {
            return returnVal;
        }
        if (index == 0) {
            return removeHead();
        }
        if (index == length - 1) {
            return removeTail();
        }
        Node* cN = this->head;
        size_t limit = this->length;
        for (size_t i = 0; i < limit; i++) {
            if (i + 1 == index) {
                returnVal = cN->next->file;
                free(cN->next);
                cN->next = cN->next->next;
                this->length--;
                return returnVal;
            }
        }
        return returnVal;
    }



    File linkedlist::LinkedList::itemAtIndex(int index) {

        Node* current = this->head;
        for (int i = 0;i < index;i++) {
            if (current->next != NULL) {
                current = current->next;
            } else {
                exit(1);
            }
        }
        return current->file;
    }



    void linkedlist::LinkedList::printList() {
        size_t limit = this->length;
        if (!this->length) {
            printf("Empty");
            return;
        }
        Node* currentNode = this->head;
        for (size_t i = 0; i < limit; i++) {
            printf("%s:%d -> ", currentNode->filename.c_str(), (int)currentNode->file.bytes.size());
            currentNode = currentNode->next;
        }
    }

