#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <math.h>

#include "Sieve.h"
//#define DEBUG

using namespace std;
typedef string::size_type MapSize;

Sieve::Sieve(int* primalityFormatingWidth){
	hasZero = false;
	tensWidth = primalityFormatingWidth;
	primalityFormatingIndex = 0;
	cycles = new Cycles();
	primes = new PrimeList();
	primeFactors = new PrimeList();
	refList = new PrimeList();

	recurrences[1] = new Recurrence();
	recurrences[3] = new Recurrence();
	recurrences[7] = new Recurrence();
	recurrences[9] = new Recurrence();
}

void Sieve::alignRecurrences(){
	for(iter_rec = recurrences.begin(); iter_rec != recurrences.end(); iter_rec++){
		iter_rec->second->align(primes);
	}
}

void Sieve::addPrime(unsigned long prime){
	primes->add(prime);
}

void Sieve::addCycle(){
	unsigned long recLen = recurrences[1]->getLength();
	unsigned long prime = primes->getValueAtIndex(recLen);
	unsigned long mod10 = prime % 10;
	unsigned long ten = ((prime - mod10) / 10);
	unsigned long square = prime * prime;

	unsigned long lastPrime = primes->getLast()->value;
	unsigned long lastPrimeMod10 = lastPrime % 10;
	unsigned long lastPrimeTen = ((lastPrime - lastPrimeMod10) / 10);

	Cycle cycle = cycles->getCycle(mod10);
	unsigned long cycleAlign = lastPrimeTen - ten;

	recurrences[cycle.mods[0]]->add( ((prime + cycleAlign) % prime) );
	unsigned long ref = prime;
	for(int i = 1, j = 0; i <= 3; i++, j++){
		ref -= ( cycle.gaps[j] + (ten * cycles->TENS_GAPS[j]) );
		recurrences[cycle.mods[i]]->add( ((ref + cycleAlign) % prime));
	}
}

void Sieve::saveTenPrimality(unsigned long &currTen){
	
	struct Formater {
		static string primalityStr(bool &hasRec, unsigned long &currTen){
			if(hasRec){
				return "	";
			}else{
				if(currTen > 999){
					return "999 ";
				}else{
					string strnum = to_string(currTen);
					if( 3 > strnum.size() )
						strnum.insert(0, 3 - strnum.size(), '0');

					return strnum+" ";
				}
			}
		}
	};

	int recKey = 1;
	string filePrimality;
	ofstream outputFile;
	
	for(int i=0; i<=3; i++){
		filePrimality = "primality_["+ to_string(recKey) +"]_"+ to_string(*tensWidth) +"_tens_per_line.txt";	
		recurrences[recKey]->checkForZeros(hasZero);
		
		outputFile.open(filePrimality, ios::app);
		filePrimality = "primality_["+ to_string(recKey) +"]_"+ to_string(*tensWidth) +"_tens_per_line.txt";	
		recurrences[recKey]->checkForZeros(hasZero);
		
		outputFile.open(filePrimality, ios::app);
		filePrimality = "primality_["+ to_string(recKey) +"]_"+ to_string(*tensWidth) +"_tens_per_line.txt";	
		recurrences[recKey]->checkForZeros(hasZero);
		
		outputFile.open(filePrimality, ios::app);
		outputFile << Formater::primalityStr(hasZero, currTen);
		if(primalityFormatingIndex == *tensWidth-1)
			outputFile << endl;
		outputFile.close();

		if(i<3) recKey += cycles->TENS_GAPS[i];
	}
	
	primalityFormatingIndex = (primalityFormatingIndex + 1) % *tensWidth;
}

void Sieve::factorize(string &jumpStartFilePath, unsigned long &numberToFactorize){
	bool isJumpStarted = false;
	if(jumpStartFilePath != ""){
		loadPreviousStates(jumpStartFilePath, numberToFactorize);
		isJumpStarted = true;
	}

	if(primes->getLength() == 0 || numberToFactorize > primes->getLast()->value){
		analyse(numberToFactorize, false, isJumpStarted); 
	}

	primes->getPrimeFactors(numberToFactorize, *primeFactors);
	if(numberToFactorize > 1){
		primeFactors->add(numberToFactorize);
	}

	#ifdef DEBUG
	clog << "remainder : " << numberToFactorize << endl;
	clog << "PRIME FACTORS : " << primeFactors->toString(",") << endl;
	clog << "RECURRENCES 1 : " << recurrences[1]->toString(",") << endl;
	clog << "RECURRENCES 3 : " << recurrences[3]->toString(",") << endl;
	clog << "RECURRENCES 7 : " << recurrences[7]->toString(",") << endl;
	clog << "RECURRENCES 9 : " << recurrences[9]->toString(",") << endl;
	#endif
}

void Sieve::analyse(unsigned long limit, bool saveTenModularPrimality, bool isJumpStarted){
	unsigned int mod10;

	if(!isJumpStarted){
		probe = 1;
		currSquare = 1;
		currTen = 0;
		currPosProbeLoop = 1;
	}

	while (probe < limit) {
		while (currPosProbeLoop <= 3) {
			probe += cycles->PROBE_LOOP[currPosProbeLoop];
			mod10 = probe % 10;
			recurrences[mod10]->checkForZeros(hasZero);
			if (probe == 3 || !hasZero) {
				if (currSquare <= probe) {
					addPrime(probe);
					currSquare = (probe == 3) ? 9 : pow(primes->getValueAtIndex(recurrences[1]->getLength()),2);
					addCycle();
				} else {
					addPrime(probe);
				}
			}
			currPosProbeLoop++;
		}
		
		if(saveTenModularPrimality)
			saveTenPrimality(currTen);
		
		alignRecurrences();
		currPosProbeLoop = 0;
		currTen++;
	}
}

void Sieve::loadPreviousStates(string &jumpStartFilePath, unsigned long &numberToFactorize){
	Link *lastPrime;
	unsigned long sqrtMaxPrime;
	unsigned int mod10;
	Boundary bound;

	loadPrimeList(jumpStartFilePath);
	lastPrime = primes->getLast();

	if(numberToFactorize > 0 && numberToFactorize <= lastPrime->value){
		probe = numberToFactorize;
	}else{
		probe = lastPrime->value;
	}
	mod10 = probe % 10;
	currTen = (probe - mod10) / 10;
	probe = (currTen * 10) + 9;
	sqrtMaxPrime = sqrt(probe);

	currPosProbeLoop = 0;
	bound = primes->getBoundingPrime(sqrtMaxPrime);
	currSquare = bound.value * bound.value;

	while( recurrences[1]->getLength() <= bound.index ) {
		addCycle();
	}
	alignRecurrences();
	
	#ifdef DEBUG
	clog << "============ loadPreviousStates ==================" << endl;
	clog << "-------------> Recurrences :" << endl;
	recurrences[1]->print(",");
	recurrences[3]->print(",");
	recurrences[7]->print(",");
	recurrences[9]->print(",");
	clog << "-------------> currSquare = " << currSquare << endl;
	clog << "-------------> probe = " << probe << endl;
	clog << "-------------> currPosProbeLoop = " << currPosProbeLoop << endl;
	clog << "-------------> currTen = " << currTen << endl;
	#endif
}

void Sieve::loadPrimeList(string &jumpStartFilePath){
	primes->loadFromFile(jumpStartFilePath);
}

void Sieve::compareCheck(string &primeListPath, bool &displaySimilarityRange) {
	refList->loadFromFile(primeListPath);
	primes->compare(*refList, displaySimilarityRange);
}

void Sieve::savePrimeList(string &primeListPath){
	#ifdef DEBUG
	clog << "============ savePrimeList ==================" << endl;
	clog << "-------------> Recurrences :" << endl;
	recurrences[1]->print(",");
	recurrences[3]->print(",");
	recurrences[7]->print(",");
	recurrences[9]->print(",");
	clog << "-------------> currSquare = " << currSquare << endl;
	clog << "-------------> probe = " << probe << endl;
	clog << "-------------> currPosProbeLoop = " << currPosProbeLoop << endl;
	clog << "-------------> currTen = " << currTen << endl;
	#endif
	
	ofstream outputFile;
	outputFile.open(primeListPath, ios::out | ios::app);
	outputFile.close();
	outputFile.open(primeListPath, ios::in | ios::trunc);
	outputFile << primes->toString("\n");
	outputFile.close();
}

void Sieve::destroy(){
	delete(cycles);
	primes->destroy();
	delete(primes);
	recurrences[1]->destroy();
}
