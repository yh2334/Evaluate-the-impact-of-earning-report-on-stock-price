#pragma once
#ifndef Sock_h
#define Sock_h
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <string>
#include "Date.h"

class Stock {
protected:
	std::string StartTime;
	std::string EndTime;
	double EPSactual;
	double EPSestimate;
	std::map<Date, double> Prices;
	std::map<Date, double> Returns;

public:
	Stock() {}
	Stock(std::string starttime, std::string endtime, double eps_act, double eps_est)
		:StartTime(starttime), EndTime(endtime), EPSactual(eps_act), EPSestimate(eps_est) {}
	Stock(const Stock& stock) : StartTime(stock.StartTime), EndTime(stock.EndTime),
		EPSactual(stock.EPSactual), EPSestimate(stock.EPSestimate), Prices(stock.Prices), Returns(stock.Returns) {} //When divide, the Price and Returns should be copied,
	virtual ~Stock() {} 

	void setPrices(const Date& date, const double& price) { Prices.insert(std::pair<Date, double>(date, price)); } 
	void setReturns(const Date& date, const double& returna) { Prices.insert(std::pair<Date, double>(date, returna)); }
	std::string getStartTime() { return StartTime; }
	std::string getEndTime() { return EndTime; }
	double getEPSactual() const { return EPSactual; }
	double getEPSestimate() const { return EPSestimate; }
	double getEPSbeat() { return (EPSactual / EPSestimate) - 1; }
	std::map<Date, double> GetReturns() { return Returns; }
	void setReturns();
	double getReturns(int t) { auto it = Returns.begin(); advance(it, t); return it->second; } //Return the t'th day's Return
	double getPrices(int t) { auto it = Prices.begin(); advance(it, t); return it->second; }

};

class Market : public Stock {
public:
	Market() {}
	Market(std::string starttime, std::string endtime, double eps_act, double eps_est)
		:Stock(starttime, endtime, eps_act, eps_est) {}
	Market(const Market& market) {StartTime = market.StartTime; EndTime = market.EndTime; EPSactual = market.EPSactual; EPSestimate = market.EPSestimate; Prices = market.Prices; Returns = market.Returns; }
	int dis(Date startdate) { int a = 0; //Calculate the distance between the starting date of Market and Stock
	auto it = Returns.begin();
	while (it->first < startdate) {
		it++;
		a++;
	}
	return a;
	}
};
#endif