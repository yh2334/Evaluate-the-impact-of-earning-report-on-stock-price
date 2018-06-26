#pragma once
#ifndef BootStrapping_h
#define BootStrapping_h
#include <iostream>
#include <string>
#include <map>
#include <list>
#include "Date.h"
#include "Stock.h"
#include "TickerBook.h"


class Bootstrapping {
private:
	double Threshold;   //To divide the TickerBook (Group)
	int Resampletime; 
	std::vector<double> AARlist = std::vector<double>(90); //The averaged AAR of Samples
	std::vector<double> CAARlist = std::vector<double>(90);//The averaged AAR of Samples

public:
	Bootstrapping() {};
	Bootstrapping(double threshold, int resampletime) { Threshold = threshold; Resampletime = resampletime; }
	Bootstrapping(const Bootstrapping& boot) { Threshold = boot.Threshold; Resampletime = boot.Resampletime; AARlist = boot.AARlist; CAARlist = boot.CAARlist; }//The AAR / CAAR should be copied.
	~Bootstrapping() {};
	std::vector<double> getAARlist() { return AARlist; }
	std::vector<double> getCAARlist() { return CAARlist; }
	void setResampletime(const int &resampletime) { Resampletime = resampletime; }
	int getResampletime() const { return Resampletime; }
	void run(Market *market, TickerBook *tickerbook);
};

#endif