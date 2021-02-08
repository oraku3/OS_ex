// ring++.h
// Data structures of a ring buffer to be used in producer and
// consumer problem
//
// The constructor (initializer) for the ring burrer is passed with an
// integer for the size of the buffer (the number of slots). 

#include "synch.h"


// class of the slot in the ring-buffer
class slot {
    public:
    slot(int id, int number);
    slot() { thread_id = 0; value = 0;};
    
    int thread_id;
    int value;
    };

class Ring {
  public:
     Ring(int sz);    // Constructor:  initialize variables, allocate space.
     ~Ring();           // Destructor:   deallocate space allocated above.


    void Put(slot *message); // Put a message the next empty slot.

    void Get(slot *message); // Get a message from the next  full slot.

  private:
     int size;             // The size of the ring buffer.
     int in, out;        // Index of slots
     slot *buffer;     // A pointer to an array for the ring buffer.
     int current;      // the current number of full slots in the butter
     Condition *full;
     Condition *empty;
     Lock *bufferlock;
};