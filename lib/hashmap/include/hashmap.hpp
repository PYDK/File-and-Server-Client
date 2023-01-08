#include "linkedlist.hpp"
#include <functional>
#include "pack109.hpp"

using namespace std;
  namespace hashmap {
  class HashMap {
    private:
      // The backbone of the hash map. This is an array of Linked List pointers.
      linkedlist::LinkedList** array;

      // The number of buckets in the array
      size_t size; 

      // Generate a prehash for an item with a given size
      unsigned long prehash(string filename);

    public:
      
      // Initialize an empty hash map, where size is the number of buckets in the array
      HashMap(size_t size);
      HashMap();
      // Free all memory allocated by the hash map
      ~HashMap();

      // Hash an unsigned long into an index that fits into a hash map
      unsigned long hash(string filename);

      // Insert item in the set. Return true if the item was inserted, false if it wasn't (i.e. it was already in the set)
      // Recalculate the load factor after each successful insert (round to nearest whole number).
      // If the load factor exceeds 70 after insert, resize the table to hold twice the number of buckets.

      bool insert(string filename, File file);

      // Remove an item from the set. Return true if it was removed, false if it wasn't (i.e. it wasn't in the set to begin with)
      bool remove(string filename);

      // Return true if the item exists in the set, false otherwise
      bool contains(string filename);

      // Resize the underlying table to the given size. Recalculate the load factor after resize
      void resize(size_t new_size);

      // Returns the number of items in the hash map
      size_t len();

      // Returns the number of items the hash map can hold before reallocating
      size_t capacity();

      File get(string filename);

      // Print Table. You can do this in a way that helps you implement your hash map.
      void print();

  };
}
