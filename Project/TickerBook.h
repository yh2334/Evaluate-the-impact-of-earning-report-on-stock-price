#pragma once
#ifndef TickerBook_h
#define TickerBook_h
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include "Date.h"
#include "Stock.h"

class TickerBook {
private:

public:
	std::map<std::string, Stock> Book;
	std::vector<std::string> BookPage;
	std::vector<double> AAR = std::vector<double>(90); //Averaged AAR of Stocks
	std::vector<double> CAAR = std::vector<double>(90); //Averaged CAAR of Stocks

	TickerBook() {}
	TickerBook(const TickerBook& tickerbook) : Book(tickerbook.Book), BookPage(tickerbook.BookPage) {}
	~TickerBook() {}

	void setBook(const std::string &ticker, const Stock &stock) { Book.insert(std::pair<std::string, Stock>(ticker, stock)); }  //Put Ticker into TickerBook
	void setBookPage(const std::string &ticker) { BookPage.push_back(ticker); } //Put Stock into TickerBook
	std::vector<double > GetAAR() const { return AAR; }
	std::vector<double> GetCAAR() const { return CAAR; }
	int getsize() const { return BookPage.size(); }
	void Divide(double threshold, TickerBook *group1, TickerBook *group2, TickerBook *group3);  //divide via pointer
	bool Compute(Market *market); //If finished return true
};

#endif

