#ifndef GetTickerList_hpp
#define GetTickerList_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>
#include <locale>
#include <iomanip>
#include "TickerBook.h"
#include "curl.h"

using namespace std;

void GetTickerList(TickerBook& StockList, string filename);
size_t write_data(void *ptr, int size, int nmemb, FILE *stream);
void *myrealloc(void *ptr, size_t size);
size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data);
string getTimeinSeconds(string Time);
int GetStockPrice(TickerBook& StockList);
int GetSPYPrice(Market& SPY);

#endif 