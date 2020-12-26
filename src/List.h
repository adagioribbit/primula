#ifndef LIST_H
#define LIST_H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct Link {
	unsigned long value;
	Link* next;
};

class List {
	private:
		Link *first, *last;
		unsigned long length;

	public:
		List ();
		void add (unsigned long val);
		bool contains (unsigned long value);
		void destroy();
		Link* getFirst ();
		Link* getLast ();
		unsigned long getLength ();
		unsigned long getValueAtIndex (unsigned long index);
		void loadFromFile(string &filePath);
		void print (string delimiter);
		string toString (string delimiter);
};
#endif
