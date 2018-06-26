#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include "Date.h"
#include "Stock.h"

using namespace std;

void Stock::setReturns() {
	auto it = Prices.begin();
	for (it; it != Prices.end(); ) {
		double lstday = it->second; 
		it++;//has been the second price
		if (it == Prices.end()) break; //make sure the last day return will not excess the bound of Prices
		else Returns.insert(pair<Date, double>(it->first,
			(it->second - lstday) / lstday));
	}
}

