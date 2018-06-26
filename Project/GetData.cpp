#include "GetData.hpp"
#include <typeinfo>
using namespace std;

size_t write_data(void *ptr, int size, int nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

struct MemoryStruct {
	char *memory;
	size_t size;
};

void *myrealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
	 NULL pointers, so we take care of it here */
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}


size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;
	
	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string getTimeinSeconds(string Time)
{
	std::tm t = {0};
	std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
	if (ssTime >> std::get_time(&t, "%Y-%m-%d %H:%M:%S"))
	{
		sprintf (time, "%lld", mktime(&t)); 
		return string(time);
	}
	else
	{
		cout << "Parse failed\n";
		return "";
	}
}

void GetTickerList(TickerBook& StockList, string filename)
{
	string ticker, act_eps, est_eps, start, end;
	ifstream infile(filename);
	while (infile.good())
	{
		getline(infile, ticker, '\t');
		getline(infile, act_eps, '\t');
		getline(infile, est_eps, '\t');
		getline(infile, start, '\t');
		getline(infile, end, '\n');
		StockList.setBook(ticker, Stock(start, end, atof(act_eps.c_str()),
			atof(est_eps.c_str())));
		StockList.setBookPage(ticker); 
	}
}

int GetStockPrice(TickerBook& StockList) //Change
{
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;
	//file pointer to create file that store the data
	FILE *fp;
	//declaration of an object CURL
	CURL *handle;
	//result of the whole process
	CURLcode result;
	curl_global_init(CURL_GLOBAL_ALL);
	handle = curl_easy_init();
	if (handle)
	{
		string sCookies, sCrumb;
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			//curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
			
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
			
			result = curl_easy_perform(handle);
			
			if (result != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}
			
			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char *ptr1 = strstr(data.memory, cKey);
			char *ptr2 = ptr1 + strlen(cKey);
			char *ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;
			sCrumb = ptr2;
			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;
			sCookies = cCookies;
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
		}
		map<string, Stock>::iterator itr = StockList.Book.begin();
		while (true)
		{
			if (itr == StockList.Book.end())
			{
				cout << "All data downloaded."<<endl;
				break;
			}
			// Input the start time and end time for certain stock.
			string Tail = " 16:00:00";
			string startTime = getTimeinSeconds(itr->second.getStartTime() + Tail);
			string endTime = getTimeinSeconds(itr->second.getEndTime() + Tail);
			
			string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
			string symbol = itr->first;
			string urlB = "?period1=";
			string urlC = "&period2=";
			string urlD = "&interval=1d&events=history&crumb=";
			string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;

			const char * cURL = url.c_str();
			const char * cookies = sCookies.c_str();
			curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
			curl_easy_setopt(handle, CURLOPT_URL, cURL);
			stringstream Info;
			struct MemoryStruct temp_ptr;
			temp_ptr.memory = NULL;
			temp_ptr.size = 0;
			string temp_line;
			
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&temp_ptr);
			result = curl_easy_perform(handle);
			
			if (result != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}
			Info.str(temp_ptr.memory);
			getline(Info, temp_line);
			for (int j = 0; j < 91; j++)
			{
				getline(Info, temp_line, ',');
				Date temp_date(temp_line);
				getline(Info, temp_line, ',');
				getline(Info, temp_line, ',');
				getline(Info, temp_line, ',');
				getline(Info, temp_line, ',');
				getline(Info, temp_line, ',');
				double temp_price = atof(temp_line.c_str());
				if (temp_price > 0) {
					itr->second.setPrices(temp_date, temp_price); 
				}
				else cout << itr->first << "'s Price is Invalid" << endl;
				itr->second.setPrices(temp_date, temp_price);
				getline(Info, temp_line, '\n');
			}
		//	cout << itr->first << endl;
			free(temp_ptr.memory);
			temp_ptr.size = 0;
			itr->second.setReturns();
			itr++;
		}
		
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}
	
	curl_easy_cleanup(handle);
	
	curl_global_cleanup();
	return 0;
}

int GetSPYPrice(Market& Spy)
{
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;
	//file pointer to create file that store the data
	FILE *fp;
	CURL *handle;
	CURLcode result;
	//curl_global_init(CURL_GLOBAL_ALL);
	handle = curl_easy_init();
	if (handle)
	{
		string sCookies, sCrumb;
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			//curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
			result = curl_easy_perform(handle);
			if (result != CURLE_OK)
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}
			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char *ptr1 = strstr(data.memory, cKey);
			char *ptr2 = ptr1 + strlen(cKey);
			char *ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;
			sCrumb = ptr2;
			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;
			sCookies = cCookies;
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
		}
		// Input the start time and end time for certain stock.
		string Tail = " 16:00:00";
		string startTime = getTimeinSeconds(Spy.getStartTime() + Tail);
		string endTime = getTimeinSeconds(Spy.getEndTime() + Tail);
		string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
		string symbol = "SPY";
		string urlB = "?period1=";
		string urlC = "&period2=";
		string urlD = "&interval=1d&events=history&crumb=";
		string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
		const char * cURL = url.c_str();
		const char * cookies = sCookies.c_str();
		curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
		curl_easy_setopt(handle, CURLOPT_URL, cURL);

		stringstream Info;
		struct MemoryStruct temp_ptr;
		temp_ptr.memory = NULL;
		temp_ptr.size = 0;
		string temp_line;
		
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&temp_ptr);
		result = curl_easy_perform(handle);
		
		if (result != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}
		
		Info.str(temp_ptr.memory);
		
		getline(Info, temp_line);// Pop out the first line.
		for (int j = 0; j < 164; j++)
		{
			getline(Info, temp_line, ',');//Date
			
			Date temp_date(temp_line.c_str());// date
			//cout << temp_date.getMonth()<< endl;
			getline(Info, temp_line, ',');//Open
			getline(Info, temp_line, ',');//High
			getline(Info, temp_line, ',');//Low
			getline(Info, temp_line, ',');//Close
			getline(Info, temp_line, ',');//Adj close
			double temp_price = atof(temp_line.c_str());//price
			getline(Info, temp_line, '\n');//Volume
			Spy.setPrices(temp_date, temp_price);			
		}
		free(temp_ptr.memory);
		temp_ptr.size = 0;
		Spy.setReturns();
		
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}
	
	curl_easy_cleanup(handle);
	
	curl_global_cleanup();
	return 0;
}
