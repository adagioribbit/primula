#ifndef RECURRENCE_H
#define RECURRENCE_H

#include "Primelist.h"

class Recurrence : public List {
	public:
		void align (PrimeList* primes);
		void checkForZeros (bool& hasZero);
		long findFirstZero ();
};

#endif
