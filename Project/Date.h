#pragma once
#include <iostream>
#include <string>

class Date {

private:
	int Year, Month, Day;
	int t;

public:
	Date() : Year(0), Month(0), Day(0) {}
	Date(int year, int month, int day) : Year(year), Month(month), Day(day) {}  
	Date(std::string date) :Year(stoi(date.substr(0, 4))), Month(stoi(date.substr(5, 2))), Day(stoi(date.substr(8, 2))){}
	Date(const Date& date) : Year(date.Year), Month(date.Month), Day(date.Day) {}
	~Date() {}

	bool operator<(const Date& obj) const;
	int getYear() const;
	int getMonth() const;
	int getDay() const;
	void setYear(int year);
	void setMonth(int month);
	void setDay(int day);
	std::string toString() const;
};