#include "List.h"
#define DEBUG

List::List () {}

Link* List::getFirst (){
	return first;
}

Link* List::getLast (){
	return last;
}

unsigned long List::getLength (){
	return length;
}

void List::add (unsigned long val) {
	Link* link = new Link();
	link->value = val;
	
	if(!first){
		first = link;
		last = link;
	}else{
		last->next = link;
		last = link;
	}
	
	length++;
}

bool List::contains (unsigned long value) {
	unsigned long i = 0;
	Link* search = first;
	
	if(value == search->value)
		return true;

	while(search->next){
		if(value == search->value) {
			return true;
		}
		search = search->next;
		i++;
	}
	return false;
}

unsigned long List::getValueAtIndex (unsigned long index) {
	unsigned long i = 0;
	Link* search = first;

	while(i < index){
		search = search->next;
		i++;
	}
	return search->value;
}

void List::print (string delimiter = "") {
	Link* recLink = first;

	if (delimiter.length() > 0) {
		while(recLink){
			cout << recLink->value << delimiter;
			recLink = recLink->next;
		}
		cout << endl;
	} else {
		while(recLink){
			cout << recLink->value << endl;
			recLink = recLink->next;
		}
	}
}

string List::toString (string delimiter = "") {
	string res;
	Link* recLink = first;

	while(recLink){
		res = res + to_string(recLink->value) + delimiter;
		recLink = recLink->next;
	}

	return res;
}

void List::loadFromFile(string &filePath) {
	string currLocale = locale("").name().substr(0,5);
	string line;
	unsigned long lastNumber, number;
	ifstream sourceFile (filePath);

	if (sourceFile.is_open()) {
		try {
			lastNumber = 0;
			while (getline(sourceFile,line)) {
				number = stoi(line);
				if(number > 2 && lastNumber < number){
					add(number);
					lastNumber = number;
				}else{
					if(currLocale == "fr_FR"){
						cerr << "La suite de nombres premiers dans " << filePath << " n'est pas strictement croissante." << endl;
					}else{
						cerr << "The prime numbers sequence in " << filePath << " is not strictly increasing." << endl;
					}
					exit(EXIT_FAILURE);
				}
			}
			sourceFile.close();
		} catch (const invalid_argument& ia) {
			if(currLocale == "fr_FR"){
				cerr << "Une des entrées dans " << filePath << " n'a pas pu être convertie en entier : " << line << endl;
			}else{
				cerr << "An entry in " << filePath << " could not be converted into an integer : " << line << endl;
			}
			exit(EXIT_FAILURE);
		}
	} else {
		if(currLocale == "fr_FR"){
			cerr << "Le fichier " << filePath << " n'a pas pu être ouvert.";
		}else{
			cerr << "File " << filePath << " could not be opened";
		}
		exit(EXIT_FAILURE);
	}
}

//void PrimeVist::compare(PrimeList &refList){
//    cout << "============ compareCheck ==================" << endl;
//	Link* prime = first;
//	Link* refPrime = refList.getFirst();
//    int padding = 0;
//
//	while(prime->next){
//		if(refPrime->next){
//			if(prime->value == refPrime->value){
//				cout << "\033[1;32m" << prime->value << " \033[0m";
//			}else{
//				cout << "\033[0;31m" << prime->value << " \033[0m";
//			}
//			refPrime = refPrime->next;
//		}else{
//			cout << "\033[0m" << prime->value << " \033[0m";
//		}
//		
//		padding++;
//		if(padding > 30){
//			padding = 0;
//			cout << endl;
//		}
//	}
//	cout << "============================================" << endl;	
//}

void List::destroy(){
	Link* currLink;
	Link* nextLink = first->next;

	while(nextLink){
		currLink = nextLink;
		nextLink = nextLink->next;
		delete(currLink);
	}
	delete(first);
}
