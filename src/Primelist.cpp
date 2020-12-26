#include "Primelist.h"

void PrimeList::compare(PrimeList &refList, bool &displaySimilarityRange){
	Link* prime = getFirst();
	Link* refPrime = refList.getFirst();
	unsigned int currSimilarityStatus;

	if(displaySimilarityRange){
		SimilarityRange comparisonRange;

		while(prime){
			getSimilarityStatus(prime, refPrime, currSimilarityStatus);
			if(comparisonRange.similarityStatus != currSimilarityStatus){
				comparisonRange.print();
				comparisonRange.start = prime->value;
				comparisonRange.similarityStatus = currSimilarityStatus;
			}else{
				comparisonRange.end = prime->value;
			}

			if(refPrime){
				refPrime = refPrime->next;
			}
			prime = prime->next;
		}
		comparisonRange.print();
	}else{
		while(prime){
			getSimilarityStatus(prime, refPrime, currSimilarityStatus);
			switch(currSimilarityStatus){
				case 0:
					clog << "\033[0m" << prime->value << " \033[0m";
					break;
				case 1:
					clog << "\033[1;32m" << prime->value << " \033[0m";
					break;
				case 2:
					clog << "\033[1;31m" << prime->value << "/" << refPrime->value << " \033[0m";
					break;
				default:
					break;
			}
			
			if(refPrime){
				refPrime = refPrime->next;
			}
			prime = prime->next;
		}
		clog << endl;
	}
}

void PrimeList::getSimilarityStatus(Link *a, Link *b, unsigned int &similarityStatus){
	if(!a || !b){
		similarityStatus = 0;
	}else if(a->value == b->value){
		similarityStatus = 1;
	}else{
		similarityStatus = 2;
	}
}

Boundary PrimeList::getBoundingPrime (unsigned long &refValue){
	Boundary upperBound;
	unsigned long index = 0;
	Link* search = getFirst();
	
	while(search->value < refValue){
		search = search->next;
		index++;
	}
	
	upperBound.index = index;
	upperBound.value = search->value;
	
	return upperBound;
}

void PrimeList::getPrimeFactors (unsigned long &numberToFactorize, PrimeList &primeFactors) {
	checkFactor(numberToFactorize, primeFactors, 2);
	checkFactor(numberToFactorize, primeFactors, 5);
	if(getLength() > 0){
		Link* currPrime = getFirst();
		
		checkFactor(numberToFactorize, primeFactors, currPrime->value);
		while(currPrime && numberToFactorize > 1 && !contains(numberToFactorize)){
			currPrime = currPrime->next;
			checkFactor(numberToFactorize, primeFactors, currPrime->value);
		}
	}
}

void PrimeList::checkFactor(unsigned long &numberToFactorize, PrimeList &primeFactors, unsigned long factor) {
    while(numberToFactorize % factor == 0){
        numberToFactorize = numberToFactorize / factor;
        primeFactors.add(factor);
    }
}
