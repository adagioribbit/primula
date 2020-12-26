#include "Loop.h"

Loop::Loop (unsigned long arr[]) : List(){
	Link* start = new Link();
	Link* end = new Link();

	for(int i = 0; arr[i]; i++){
	    add(arr[i]);
	}
	start = getFirst();
	end = getLast();
	end->next = start;
};
