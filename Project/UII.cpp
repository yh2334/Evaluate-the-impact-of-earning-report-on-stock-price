#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread> 
#include "GetData.hpp"
#include "BootStrapping.h"
#include "ShowData.h"
#include "UII.h"


using namespace std;

void UserInterface() {
	int resample = 0;
	double threshold = 0.0;
	int choice2 = 0;
	string choice3;
	string choice4;
	Market Spy("2017-09-01", "2018-04-27", 0, 0);
	string filename = "stocks.txt";
	TickerBook StockList;
	TickerBook group1;
	TickerBook group2;
	TickerBook group3;
	Bootstrapping Boot1(threshold, resample), Boot2(threshold, resample), Boot3(threshold, resample);

	while (true) {
		cout << "================================================================================================================== " << endl <<
			"================================================================================================================== " << endl <<
			"Team Project" << endl <<
			"Created by Group 6. " << endl <<
			"Copyright @2018 FRE-NYU-Tandon-Group6. All rights reserved. " << endl <<
			"Produced by Yichen Wang, Elizabeth Zhang, Xingyao Jie, Tiffany Fan, Yixuan Huang. " << endl <<
			"================================================================================================================== "<<endl <<
			"***************************************************" << endl <<
			"******* Welcome! Please choose your choise: *******" << endl << endl <<
			"1: Retrieve historical price data for all stocks" << endl <<
			"2: Pull information for one stock from one group" << endl <<
			"3: Show AAR for one group" << endl <<
			"4: Show CAAR for one group" << endl <<
			"5: Show the Excel graph with CAAR for all 3 groups" << endl <<
			"6: EXIT" << endl << endl <<
			"****** Please choose 1 for the first time! ********" << endl <<
			"***************************************************" << endl;
		int choice;
		cin >> choice;

		switch (choice) {
		case 1: {
			cout << "Start Data Downloading." << endl;
			GetTickerList(StockList, filename);
			GetStockPrice(StockList);
			GetSPYPrice(Spy);
			cout << "Please enter the Threshold: " << endl;
			cin >> threshold;
			StockList.Divide(threshold, &group1, &group2, &group3);
			cout << "The Beat Group has " << group1.BookPage.size() << " Stocks" << endl;
			cout << "The Meet Group has " << group2.BookPage.size() << " Stocks" << endl;
			cout << "The Miss Group has " << group3.BookPage.size() << " Stocks" << endl;
			cout << "Start the Bootstrapping Process." << endl;
			cout << "Please Enter the Resampling Time:" << endl;
			cin >> resample;
			Boot1.setResampletime(resample);
			Boot2.setResampletime(resample);
			Boot3.setResampletime(resample);

			cout << "Start Resampling." << endl;
			
			thread t1(&Bootstrapping::run, &Boot1, &Spy, &group1);
			
			thread t2(&Bootstrapping::run, &Boot2, &Spy, &group2);
			
			thread t3(&Bootstrapping::run, &Boot3, &Spy, &group3);

			t1.join(); t2.join(); t3.join();

			cout << "Data is Ready !" << endl;
			continue;
		}
		case 2: {
			string a;
			cout << "Which Stock you want to see? Please enter the Ticker of it:" << endl;
			cin >> a;
			auto ita = find(group1.BookPage.begin(), group1.BookPage.end(), a);
			if (ita == group1.BookPage.end()) {
				auto itb = find(group2.BookPage.begin(), group2.BookPage.end(), a);
				if (itb == group2.BookPage.end()) {
					auto itc = find(group3.BookPage.begin(), group3.BookPage.end(), a);
					if (itc == group3.BookPage.end()) {
						cout << "This Stock is not in the Stock List. Please try another one." << endl;
					}
					else {
						cout << a << " is in the Miss Group." << endl;
					}

				}
				else {
					cout << a << " is in the Miss Group." << endl;
				}
			}
			else {
				cout << a << " is in the Beat Group." << endl;

			}
			cout << "What information do you want to see?" << endl;
			cout << "1. Price data." << endl;
			cout << "2. Return data." << endl;
			cout << "3. No, Please Back to Menu." << endl;
			cin >> choice2;
			if (choice2 == 1) {
				auto itf = StockList.Book.find(a);
				for (int tt = 0; tt < 90; tt++) {
					cout << "The " << tt + 1 << "th Day's Price is " << itf->second.getPrices(tt) << endl;
				}
			}
			else if (choice2 == 2) {
				auto itf = StockList.Book.find(a);
				for (int tt = 0; tt < 90; tt++) {
					cout << "The " << tt + 1 << "th Day's Return is " << itf->second.getReturns(tt) << endl;
				}
			}
			else if (choice2 == 3) {
				cout << "emm.. We do not have this function now." << endl;
			}
			continue;
		}
		case 3: {
			cout << "Which group's AAR you want? " << endl;
			cin >> choice3;
			if (choice3 == "Beat") {
				for (int t1 = 0; t1 < 90; t1++) {
					cout << Boot1.getAARlist()[t1] << endl;
				}

			}
			else if (choice3 == "Meet") {
				for (int t2 = 0; t2 < 90; t2++) {
					cout << Boot2.getAARlist()[t2] << endl;
				}
			}
			else if (choice3 == "Miss") {
				for (int t3 = 0; t3 < 90; t3++) {
					cout << Boot3.getAARlist()[t3] << endl;
				}
			}
			else
			{
				cout << "Invalid choice." << endl;
			}
			continue;
		}
		case 4: {
			cout << "Which group's CAAR you want? " << endl;
			cin >> choice4;
			if (choice4 == "Beat") {
				for (int t1 = 0; t1 < 90; t1++) {
					cout << Boot1.getCAARlist()[t1] << endl;
				}

			}
			else if (choice4 == "Meet") {
				for (int t2 = 0; t2 < 90; t2++) {
					cout << Boot2.getCAARlist()[t2] << endl;
				}

			}
			else if (choice4 == "Miss") {
				for (int t3 = 0; t3 < 90; t3++) {
					cout << Boot3.getCAARlist()[t3] << endl;
				}

			}
			else
			{
				cout << "Invalid choice." << endl;
			}
			continue;
			
		}
		case 5: {
			ShowPic(Boot1.getCAARlist(), Boot2.getCAARlist(), Boot3.getCAARlist());
			continue;
		}
		case 6: {
			cout << "Press Any Button to Exit the program." << endl;
			cin.ignore();
			exit(0);
		}
		}


	}



}