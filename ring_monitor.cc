// ring.cc
//	Routines to implement a ring buffer for producer and consumer 
//      problem.
//	

/*extern "C" {
#include <stdio.h>
extern int exit(int st);
}*/

#include "ring_monitor.h"
#include "synch.h"

#define BUFF_SIZE 3


//----------------------------------------------------------------------
// slot::slot
// 	The constructor for the slot class.  
//----------------------------------------------------------------------

slot::slot(int id, int number)
{
    thread_id = id;
    value = number;
}



//----------------------------------------------------------------------
// Ring::Ring
// 	The constructor for the Ring class.  Note that it doesn't have a
// 	return type.
//
// 	"sz" -- maximum number of elements in the ring buffer at any time
//----------------------------------------------------------------------

Ring::Ring(int sz)
{
    if (sz < 1) {
	fprintf(stderr, "Error: Ring: size %d too small\n", sz);
	exit(1);
    }

    // Initialize the data members of the ring object.
    size = sz;
    in = 0;
    out = 0;
    current = 0;
    buffer = new slot[size]; //allocate an array of slots.
    full = new Condition("notfull");
    empty = new Condition("notempty");
    bufferlock = new Lock("bufferlock");
}

//----------------------------------------------------------------------
// Ring::~Ring
// 	The destructor for the Ring class.  Just get rid of the array we
// 	allocated in the constructor.
//----------------------------------------------------------------------

Ring::~Ring()
{
    // Some compilers and books tell you to write this as:
    //     delete [size] stack;
    // but apparently G++ doesn't like that.

    delete [] buffer;
}

//----------------------------------------------------------------------
// Ring::Put
// 	Put a message into the next available empty slot. We assume the
//       caller has done necesaary synchronization.
//
//	"message" -- the message to be put in the buffer
//----------------------------------------------------------------------

void
Ring::Put(slot *message)
{
    bufferlock->Acquire();
    while(current == BUFF_SIZE) full->Wait(bufferlock);
    buffer[in].thread_id = message->thread_id;
    buffer[in].value = message->value;
    in = (in + 1) % size;
    current++;
    //bufferlock->Release();
    empty->Signal(bufferlock);
    bufferlock->Release();
}

//----------------------------------------------------------------------
// Ring::Get
// 	Get a message from the next full slot. We assume the
//       caller has done necesaary synchronization.
//
//	"message" -- the message from  the buffer
//----------------------------------------------------------------------

void
Ring::Get(slot *message)
{
    bufferlock->Acquire();
    while(current == 0) empty->Wait(bufferlock);
    message->thread_id = buffer[out].thread_id;
    message->value = buffer[out].value;
    out = (out + 1) % size;
    current--;
    //bufferlock->Release();
    full->Signal(bufferlock);
    bufferlock->Release();
}