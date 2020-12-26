#ifndef PRIMELIST_H
#define PRIMELIST_H

#include "List.h"

struct Boundary {
	unsigned long index;
	unsigned long value;
};

struct SimilarityRange {
	unsigned long start;
	unsigned long end;
	unsigned int similarityStatus;
	void print(){
		switch(similarityStatus){
			case 0:
				clog << "\033[0m" << start;
				if(end > start)
					clog << ".." << end;
				clog << " : Out of range\033[0m" << endl;
				break;
			case 1:
				clog << "\033[1;32m" << start;
				if(end > start)
					clog << ".." << end;
				clog << " : Similarity\033[0m" << endl;
				break;
			case 2:
				clog << "\033[1;31m" << start;
				if(end > start)
					clog << ".." << end;
				clog << " : Dissimilarity\033[0m" << endl;
				break;
			default:
				break;
		};
	};
};

class PrimeList : public List {
	public:
		void checkFactor(unsigned long &numberToFactorize, PrimeList &primeFactors, unsigned long factor);
		void compare(PrimeList &refList, bool &displaySimilarityRange);
		void getPrimeFactors(unsigned long &numberToFactorize, PrimeList &primeFactors);
		Boundary getBoundingPrime (unsigned long &refValue);
		void getSimilarityStatus(Link *a, Link *b, unsigned int &similarityStatus);
		void loadPrimeList (string &filePath);
};

#endif
