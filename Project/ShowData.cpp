#include"ShowData.h"
#include <vector>
#include <iostream>
using namespace std;

void ShowPic(const vector<double>& Group1_CAAR, const vector<double>& Group2_CAAR, const vector<double>& Group3_CAAR)
{
	Vector<double, long> CAARx1(90);
	Vector<double, long> CAARx2(90);
	Vector<double, long> CAARx3(90);
	Vector<double, long> CAARy1(90);
	Vector<double, long> CAARy2(90);
	Vector<double, long> CAARy3(90);
	CAARx1[CAARx1.MinIndex()] = -59.0;
	CAARx2[CAARx2.MinIndex()] = -59.0;
	CAARx3[CAARx3.MinIndex()] = -59.0;
	for (long i = CAARx1.MinIndex() + 1; i <= CAARx1.MaxIndex(); i++)
	{
		CAARx1[i] = CAARx1[i - 1] + 1;
		CAARx2[i] = CAARx2[i - 1] + 1;
		CAARx3[i] = CAARx3[i - 1] + 1;
	}

	for (long i = CAARy1.MinIndex(); i <= CAARy1.MaxIndex(); i++)
	{
		CAARy1[i] = Group1_CAAR[i - 1];
		CAARy2[i] = Group2_CAAR[i - 1];
		CAARy3[i] = Group3_CAAR[i - 1];
	}

	list<string> labels;
	list<Vector<double, long> > yyy;
	labels.push_back("Beat");
	labels.push_back("Meet");
	labels.push_back("Miss");

	yyy.push_back(CAARy1);
	yyy.push_back(CAARy2);
	yyy.push_back(CAARy3);

	printInExcel(CAARx1, labels, yyy, "CAAR", "T/days", "return");

}