#ifndef H_SIEVE
#define H_SIEVE

#include <fstream>
#include <map>
#include <math.h>
#include <chrono>

#include "Loop.h"
#include "Primelist.h"
#include "Recurrence.h"

typedef string::size_type MapSize;

struct Cycle {
	int mods[4];
	int gaps[3];
};

struct Cycles {
	Cycle CYCLE_1 = {{1,3,7,9},{0,0,0}};
	Cycle CYCLE_3 = {{3,9,1,7},{0,2,0}};
	Cycle CYCLE_7 = {{7,1,9,3},{2,2,2}};
	Cycle CYCLE_9 = {{9,7,3,1},{2,4,2}};
	int TENS_GAPS[3] = {2,4,2};
	int PROBE_LOOP[4] = {2,2,4,2};
	
	Cycle getCycle(int mod10){
		switch(mod10){
			case 1:
				return CYCLE_1;
				break;
			case 3:
				return CYCLE_3;
				break;
			case 7:
				return CYCLE_7;
				break;
			case 9:
				return CYCLE_9;
				break;
		}
	}
};

class Sieve {	
	private:
		time_t analyseStart, analyseEnd;
		int primalityFormatingIndex;
		unsigned int currSquare, currPosProbeLoop;
		int* tensWidth;
		unsigned long probe, currTen;
	public:
		bool hasZero;
		Cycles* cycles;
		PrimeList *primes, *refList, *primeFactors;
		//Loop* probeloop;
		List* zeros; 
		map<int, Recurrence*> recurrences;
		map<int, Recurrence*>::iterator iter_rec;

		Sieve(int* zeroFormatingWidth);
		void addCycle();
		void addPrime(unsigned long prime);
		void alignRecurrences();
		void analyse(unsigned long limit, bool saveTenModularPrimality, bool isJumpStarted);
		void compareCheck(string &primeListPath, bool &displaySimilarityRange);
		void factorize(string &jumpStartFilePath, unsigned long &numberToFactorize);
		void destroy();
		void savePrimeList(string &primeListPath);
		void loadPreviousStates(string &jumpStartFilePath, unsigned long &numberToFactorize);
		void loadPrimeList(string &jumpStartFilePath);
		void saveTenPrimality(unsigned long &currTen);
};

#endif
