#include <string.h>
#include "pack109.hpp"

namespace linkedlist{
    class Node {
        public:
            string filename;
            File file;
            size_t length;
            linkedlist::Node* next;
            Node();
            Node(string filename, File file);
            ~Node();
    };
    
}
