#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include "Date.h"
#include "TickerBook.h"
#include "Stock.h"
#include "Date.h"

using namespace std;


void TickerBook::Divide(double threshold, TickerBook *group1, TickerBook *group2, TickerBook *group3) {
	auto it = BookPage.begin();
	for (it; it != BookPage.end(); it++) {
		if ((Book.find(*it)->second).getEPSbeat() >= threshold && it != BookPage.end()) {
			group1->setBookPage(*it);
			auto ita = Book.find(*it);
			group1->setBook(*it, ita->second);
		}
		else if ((Book.find(*it)->second).getEPSbeat() > -1 * threshold && (Book.find(*it)->second).getEPSbeat() < threshold && it != BookPage.end()) {
			group2->setBookPage(*it);
			auto itb = Book.find(*it);
			group2->setBook(*it, itb->second);
		}
		else if ((Book.find(*it)->second).getEPSbeat() <= -1 * threshold &&
			it != BookPage.end()) {
			group3->setBookPage(*it);
			auto itc = Book.find(*it);
			group3->setBook(*it, itc->second);
		}
	}

}

bool TickerBook::Compute(Market *market) { //It can be Pass by Value
	for (auto it = BookPage.begin(); it != BookPage.end(); it++) { //For every Stock
		int a = 0;
		auto itb = Book.find(*it);
		int dis = market->dis(itb->second.GetReturns().begin()->first);  //Calculate the Market starting date's distance with this stock
		for (int t = 0; t < 90; t++) {
			if (_isnan(itb->second.getReturns(t)) || _isnan(market->getReturns(t + dis))) { // Make sure the return will not be NAN
				AAR[t] = AAR[t];
			}
			else {
				AAR[t] = AAR[t] * a / (a + 1) + itb->second.getReturns(t) / (a + 1) 
					- market->getReturns(t + dis) / (a + 1);// The market return should be the t + dis day
			}
			if (t == 0) {
				CAAR[t] = AAR[t];
			}
			else {
				CAAR[t] = AAR[t] + CAAR[t - 1];
			}
		}
		a = a + 1;
	}
	return true;
}
//This Part's Speed is Really Low;