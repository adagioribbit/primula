// TODO :
// 	- Renommer le projet Primula
//	- Supprimer le contenu du fichier s'il existe avant la sauvegarde dans saveTenPrimality()
//	- Déplacer loadPrimeList() et savePrimeList() de Sieve vers PrimeList
//	- Interompre le main en cas de combinaison inappropriée d'arguments
//	- Repasser un coup de Valgrind sur différentes combinaisons d'arguments

#include "main.h"

static void showUsage(string& currLocale) {
	if(currLocale == "fr_FR"){
		cerr	<< "Usage: ./sieve <option(s)>" << endl
				<< "Options:" << endl
				<< "\t?, -?, -h, --help\t\tAfficher cette aide" << endl << endl
				<< "\t-l, --limit \e[4mLIMITE\e[0m\t\tDéfinir la limite supérieure de l'analyse" << endl
				<< "\t\t\t\t\t\e[4mLIMITE = entier positif strictement supérieur à 3\e[0m (défaut: 100)" << endl << endl
				<< "\t-f, --factorize \e[4mNOMBRE\e[0m\t\tTrouver les facteurs premiers du nombre" << endl
				<< "\t\t\t\t\t\e[4mNOMBRE = nombre à factoriser\e[0m" << endl << endl
				<< "\t-p, --print\t\t\tAfficher la liste des premiers au format CSV dans la sortie standard" << endl << endl
				<< "\t-z, --zeros \e[4mLARGEUR\e[0m\t\tSauvegarder la primalité des dizaines analysées pour chaque modulo 10 significatif" << endl
				<< "\t\t\t\t\t\e[4mLARGEUR = nombre de dizaines par ligne\e[0m (défaut: 21)" << endl << endl
				<< "\t-t, --timer\t\t\tAfficher le temps de traitement" << endl << endl
				<< "\t-s, --save-prime-list \e[4mCHEMIN\e[0m\tSauvegarder la liste des nombres premiers en fin d'analyse" << endl
				<< "\t\t\t\t\t\e[4mCHEMIN = chemin du fichier de sauvegarde\e[0m" << endl << endl
				<< "\t-j, --jump-start \e[4mCHEMIN\e[0m\t\tCharger la liste des nombres premiers issue d'une précédente analyse" << endl
				<< "\t\t\t\t\t\e[4mCHEMIN = chemin absolu du fichier\e[0m" << endl << endl
				<< "\t-c, --compare \e[4mCHEMIN\e[0m\t\tEn fin d'analyse, comparer la liste des premiers à une liste de référence" << endl
				<< "\t\t\t\t\t\e[4mCHEMIN = chemin absolu du fichier\e[0m" << endl << endl;
	}else{
		cerr	<< "Usage: ./sieve <option(s)>" << endl
				<< "Options:" << endl
				<< "\t?, -?, -h, --help\t\tPrint this usage page" << endl << endl
				<< "\t-l, --limit \e[4mLIMIT\e[0m\t\tSet analysis upper bound" << endl
				<< "\t\t\t\t\t\e[4mLIMIT = positive integer strictly superior to 3 (default: 100)\e[0m" << endl << endl
				<< "\t-f, --factorize \e[4mNUMBER\e[0m\t\tCompute a number prime factors" << endl
				<< "\t\t\t\t\t\e[4mNUMBER = number to factorize\e[0m" << endl << endl
				<< "\t-p, --print\t\t\tPrint prime list in CSV format to standard output" << endl << endl
				<< "\t-z, --zeros \e[4mWIDTH\e[0m\t\tSave analysed tens primality for each relevant modulo 10" << endl
				<< "\t\t\t\t\t\e[4mWIDTH = number of tens per line (default: 21)\e[0m" << endl << endl
				<< "\t-t, --timer\t\t\tPrint processing time" << endl << endl
				<< "\t-s, --save-prime-list \e[4mFILEPATH\e[0m\tSave prime list" << endl
				<< "\t\t\t\t\t\e[4mFILEPATH = save file path\e[0m" << endl << endl
				<< "\t-j, --jump-start \e[4mFILEPATH\e[0m\tLoad the prime list from a previous analysis" << endl
				<< "\t\t\t\t\t\e[4mFILEPATH = absolute file path\e[0m" << endl << endl
				<< "\t-c, --compare \e[4mFILEPATH\e[0m\tCompare resulting primes list to a reference list of primes" << endl
				<< "\t\t\t\t\t\e[4mFILEPATH = absolute file path\e[0m" << endl << endl;
	}
}

void printPrimeList(Sieve& sieve, string& currLocale){
	if(currLocale == "fr_FR"){
		clog << "\033[1;35m=================== NOMBRES PREMIERS ===================" << endl;
	}else{
		clog << "\033[1;35m==================== PRIME NUMBERS =====================" << endl;
	}
	sieve.primes->print(",");
	clog << "=======================================================\033[0m" << endl;
}

void printTimingStatistics(Sieve& sieve, fsec& fs, string& currLocale){
	ms d = std::chrono::duration_cast<ms>(fs);
	unsigned long total = d.count();
	unsigned long length = sieve.primes->getLength();
	double average = total / (double) length;

	if(currLocale == "fr_FR"){
		clog << "\033[1;34m=================== TEMPS DE CALCUL MOYEN =========================" << endl;
		clog << "\tTemp total = " << fs.count() << " secondes (" << total << " millisecondes)\n";
		clog << "\tNombre de premiers = " << length << "\n";
		clog << "\tTemp moyen par premier = " << to_string(average) << " millisecondes\n";
	}else{
		clog << "\033[1;34m=================== AVERAGE COMPUTATION TIME =========================" << endl;
		clog << "\tTotal runtime = " << fs.count() << " seconds (" << total << " milliseconds)\n";
		clog << "\tTotal primes = " << length << "\n";
		clog << "\tAverage time per prime = " << to_string(average) << " milliseconds\n";
	}
	clog << "======================================================================\033[0m" << endl;
}

int main(int argc, char* argv[]) {
	string currLocale = locale("").name().substr(0,5);
	int limit = 100;
	unsigned long numberToFactorize = 0;
	bool displayTimer = false;
	bool saveTenModularPrimality = false;
	bool printPrimes = false;
	bool averageNext = false;
	bool savePrimes = false;
	bool jumpStart = false;
	bool compareCheck = false;
	bool displaySimilarityRange = true;
	int tensWidth = 21;
	string savePrimesFilePath = getenv("HOME");
	string jumpStartFilePath = "";
	string compareCheckFilePath = "";

	if(argc > 0){
		string lastArg;
		for (int i = 1; i < argc; ++i) {
			// Deal with options
			string arg = argv[i];
			if ((arg == "?") || (arg == "-?") || (arg == "-h") || (arg == "--help")) {
				showUsage(currLocale);
				return 0;
			}
			else if ((arg == "-t") || (arg == "--timer")) {
				displayTimer = true;
			}
			else if ((arg == "-p") || (arg == "--print")) {
				printPrimes = true;
			}
			else if ((arg == "-s") || (arg == "--save-prime-list")) {
				savePrimes = true;
			}
			else if ((arg == "-l") || (arg == "--limit")) {
				if (i + 1 > argc) {
					if(currLocale == "fr_FR"){
						cerr << "L'option -l (--limit) requiert un argument." << endl;
					}else{
						cerr << "-l (--limit) option requires one argument." << endl;
					}
					return 1;
				} 
			}
			else if ((arg == "-f") || (arg == "--factorize")) {
				if (i + 1 > argc) {
					if(currLocale == "fr_FR"){
						cerr << "L'option -f (--factorize) requiert un argument." << endl;
					}else{
						cerr << "-f (--factorize) option requires one argument." << endl;
					}
					return 1;
				} 
			}
			else if ((arg == "-z") || (arg == "--zeros")) {
				saveTenModularPrimality = true;
				if (i + 1 > argc) {
					if(currLocale == "fr_FR"){
						cerr << "L'option -z (--zeros) requiert un argument." << endl;
					}else{
						cerr << "-z (--zeros) option requires one argument." << endl;
					}
					return 1;
				}
			} else if ((arg == "-j") || (arg == "--jump-start")) {
				jumpStart = true;
				if (i + 1 > argc) {
					if(currLocale == "fr_FR"){
						cerr << "L'option -j (--jump-start) requiert un argument." << endl;
					}else{
						cerr << "-j (--jump-start) option requires one argument." << endl;
					}
					return 1;
				}
			} else if ((arg == "-c") || (arg == "--compare")) {
				compareCheck = true;
				if (i + 1 > argc) {
					if(currLocale == "fr_FR"){
						cerr << "L'option -c (--compare) requiert un argument." << endl;
					}else{
						cerr << "-c (--compare) option requires one argument." << endl;
					}
					return 1;
				}
			}

			// Deal with arguments
			if(arg.substr(0,1) != "-"){
				if (lastArg == "-l" || lastArg == "--limit") {
					try{
						limit = stoi(argv[i]);
					} catch (const invalid_argument& ia) {
						if(currLocale == "fr_FR"){
							cerr << "L'option -l (--limit) requiert un entier pour argument." << endl;
						}else{
							cerr << "-l (--limit) option requires an integer as argument." << endl;
						}
						return 1;
					}
				}
				else if (lastArg == "-f" || lastArg == "--factorize") {
					try{
						numberToFactorize = stoi(argv[i]);
					} catch (const invalid_argument& ia) {
						if(currLocale == "fr_FR"){
							cerr << "L'option -f (--factorize) requiert un entier pour argument." << endl;
						}else{
							cerr << "-f (--factorize) option requires an integer as argument." << endl;
						}
						return 1;
					}
				}
				else if (lastArg == "-z" || lastArg == "--zeros") {
					try{
						tensWidth = stoi(argv[i]);
					} catch (const invalid_argument& ia) {
						if(currLocale == "fr_FR"){
							cerr << "L'option -z (--zeros) requiert un entier pour argument." << endl;
						}else{
							cerr << "-z (--zeros) option requires an integer as argument." << endl;
						}
						return 1;
					}
				}
				else if (lastArg == "-s" || lastArg == "--save-prime-list") {
					savePrimesFilePath = arg;
				}
				else if (lastArg == "-j" || lastArg == "--jump-start") {
					if (FILE *file = fopen(arg.c_str(), "r")) {
						fclose(file);
						jumpStartFilePath = arg;
					} else {							
						if(currLocale == "fr_FR"){
							cerr << "Le fichier " << arg << " n'existe pas." << endl;
						}else{
							cerr << "File " << arg << " does not exist." << endl;
						}
						return 1;
					}
				}
				else if (lastArg == "-c" || lastArg == "--compare") {
					if (FILE *file = fopen(arg.c_str(), "r")) {
						fclose(file);
						compareCheckFilePath = arg;
					} else {							
						if(currLocale == "fr_FR"){
							cerr << "Le fichier " << arg << " n'existe pas." << endl;
						}else{
							cerr << "File " << arg << " does not exist." << endl;
						}
						return 1;
					}
				}
			}

			lastArg = arg;
		}
	}
	
	if(currLocale == "fr_FR"){
		clog << "\033[1;32m=================== PARAMETRES ===================" << endl;
	}else{
		clog << "\033[1;32m=================== PARAMETERS ===================" << endl;
	}
	clog << "\tlimit\t\t\t= " << limit << endl;
	clog << "\tfactorize\t\t= " << numberToFactorize << endl;
	clog << "\tdisplayTimer\t\t= " << (displayTimer ? "true" : "false") << endl;
	clog << "\tsaveTenModularPrimality\t= " << (saveTenModularPrimality ? "true" : "false") << endl;
	clog << "\tprintPrimes\t\t= " << (printPrimes ? "true" : "false") << endl;
	clog << "\tsavePrimes\t\t= " << (savePrimes ? savePrimesFilePath : "false") << endl;
	clog << "\tjumpStart\t\t= " << (jumpStart ? jumpStartFilePath : "false") << endl;
	clog << "\tcompareCheck\t\t= " << (compareCheck ? compareCheckFilePath : "false") << endl;
	clog << "==================================================\033[0m" << endl;

	Sieve sieve(&tensWidth);

	auto t0 = Time::now();
	
	if(numberToFactorize > 0){
		sieve.factorize(jumpStartFilePath, numberToFactorize);
		if(printPrimes){
			cout << sieve.primeFactors->toString(",") << endl;
		}
		return 0;
	}else{
		if(jumpStart)
			sieve.loadPreviousStates(jumpStartFilePath, numberToFactorize);
	
		sieve.analyse(limit, saveTenModularPrimality, jumpStart);
	}


	auto t1 = Time::now();
	fsec fs = t1 - t0;
	
	if(printPrimes)
		printPrimeList(sieve, currLocale);
	
	if(displayTimer)
		printTimingStatistics(sieve, fs, currLocale);
	
	if(compareCheck)
		sieve.compareCheck(compareCheckFilePath, displaySimilarityRange);

	if(savePrimes)
		sieve.savePrimeList(savePrimesFilePath);

	sieve.destroy();
	return 0;
}

// ===== WITH MEMORY LEAKS ==========================
// hundred: 0.227185 ms
// thousand: 1.75441 ms
// ten thousand: 17.2161 ms
// hundred thousand: 248.698 ms.
// million : 5713 ms (5.71312 sec.)
// ten million : 160547 ms. (2 min. 40.547 sec.)

// ===== WITH WAY FEWER MEMORY LEAKS ==========================
// hundred: 0.204217 ms
// thousand: 1.39004 ms
// ten thousand: 15.2792 ms
// hundred thousand: 148.225 ms.
// million : 2400 ms (2.40087 sec.)
// ten million : 73188 ms. (1 min. 13.188 sec.)

// ===== WITHOUT MEMORY LEAKS ==========================
// hundred: 0.202865 ms
// thousand: 1.27729 ms
// ten thousand: 15.1754 ms
// hundred thousand: 153.354 ms.
// million : 2346 ms (2.34617 sec.)
// ten million : 64599 ms. (1 min. 4.599 sec.)
// hundred million : 2454688 ms. (40 min. 54.69 secondes)

// ===== AVERAGE NEXT PRIME COMPUTATION TIME =============
// 0-100			= 0 / 27			= 0.000000 ms
// 0-1 000			= 1 / 167			= 0.005988 ms
// 0-10 000			= 15 / 1229			= 0.012205 ms
// 0-100 000		= 164 / 9591		= 0.017099 ms
// 0-1 000 000		= 2371 / 78497		= 0.030205 ms
// 0-10 000 000		= 72846 / 664577	= 0.109613 ms
// 0-100 000 000	= 2469363 / 5761454 = 0.428601 ms

