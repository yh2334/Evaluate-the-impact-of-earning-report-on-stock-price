#include <iostream>
#include <string>
#include <map>
#include <list>
#include <time.h>
#include <vector>
#include <cmath>
#include "Date.h"
#include "TickerBook.h"
#include "Stock.h"
#include "BootStrapping.h"

using namespace std;

void swap(int *a, int *b) { 
	int temp = *a;
	*a = *b;
	*b = temp;
	return;
}

void Bootstrapping::run(Market *market, TickerBook *tickerbook) {
	srand(time(NULL));
	for (int i = 0; i < Resampletime; i++) {
		int a = tickerbook->getsize();
		vector<int> rs = vector<int>(a);
		for (int j = 0; j < a; ++j) rs[j] = j;  //Create [0, 1, ..., a - 1]
		for (int j = a - 1; j >= 1; --j) 
			swap(rs[j], rs[rand() % j]); //Swap from the last number with random index before to the second.
		TickerBook newgroup; //Create the Sample TickerBook
		for (int j = 0; j < 60; j++) {
			vector<string>::iterator it = tickerbook->BookPage.begin();
			advance(it, rs[j]); //Push the iterator by random step
			auto itr = tickerbook->Book.find(*it); 
			newgroup.setBook(itr->first, itr->second);
			newgroup.setBookPage(*it);
		}
		if (newgroup.Compute(market)) { //Finish compute the AAR / CAAR of the sample
			for (int t = 0; t < 90; t++) {
				AARlist[t] = AARlist[t] * i / (i + 1) + 
					newgroup.GetAAR()[t] / (i + 1);
				CAARlist[t] = CAARlist[t] * i / (i + 1) + 
					newgroup.GetCAAR()[t] / (i + 1);
			}
		}
	}
	cout << "This Group is Done." << endl;
	return;
}