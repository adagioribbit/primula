#include "Recurrence.h"

void Recurrence::align (PrimeList *primes) {
	Link *recLink, *primeLink;

	recLink = getFirst();
	primeLink = primes->getFirst();

	while(recLink != NULL){
		recLink->value = (recLink->value + 1) % primeLink->value;
		recLink = recLink->next;
		primeLink = primeLink->next;
	}
};

void Recurrence::checkForZeros (bool &hasZero) {
	hasZero = false;
	Link* recLink = getFirst();
	while(!hasZero && recLink != NULL){
		if(recLink->value == 0)
			hasZero = true;
		recLink = recLink->next;
	}
};

long Recurrence::findFirstZero () {
	long res = -1;
	Link* recLink = getFirst();

	while(recLink != NULL){
		if(recLink->value == 0) break;
		res++;
		recLink = recLink->next;
	}

	return res;
};
