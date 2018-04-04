#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iomanip>
#include <stdlib.h>
#include <limits>
#include <unistd.h>
#include <functional>
#include <fstream>
#include <omp.h>
#include <utility>
#include <limits.h>		//manche unnoetig

using namespace std;

void Usage()
{cout<<"USAGE"<<endl;}
struct Event {
	
	long double timestamp;
	unsigned int station;
	
	};



int main(int argc, char*argv[])
{
	string outputDateiName;
	vector <string> dateiName;
	

	
	int maxTimestampsAtOnce = INT_MAX;	
	int maxCoincidents=INT_MAX;
	




	// Einlesen der Zusatzinformationen aus der Konsole
	bool verbose = false;	
	int ch;

	int b = 100;

	
	cout << fixed << setprecision(9);		//gibt double auf 9 Nachkommastellen aus
	
	//Lesen der Argumente von der Kommandozeile
	while ((ch = getopt(argc, argv, "svm:c:o:b:C:h?"))!=EOF)
	{
		switch ((char)ch)
		{
		case 'v': verbose = true;
			break;
		case 'o':  outputDateiName = optarg;
			if (verbose){printf("Output wurde gewaehlt. %s \n", outputDateiName.c_str());}
			break;
		case 'h': Usage();
			return -1;
			return -1;
			break;
		
		case 'b': b = atoi(optarg);
			break;

	
		case 'm': maxTimestampsAtOnce = atoi(optarg);
				if (verbose) {cout<<"Es werden: "<<maxTimestampsAtOnce<<" verarbeitet"<<endl;}
			break;
			
		case 'C':maxCoincidents=atoi(optarg);
				if(verbose){cout<<"Hoechstens: "<<maxCoincidents<<endl;}
			break;
			

		}
	}	
		
	
	vector<Event >  oneCoincidence;	
	vector<vector<Event> > allCoincidents;
	
	cout<<"z"<<endl;
	for (int i=0; i<10;i++){
		
		cout<<"y"<<endl;
		Event newEvent;
		
		newEvent.timestamp=1502047690.000000023;
		newEvent.station=i;
		
		oneCoincidence.push_back(newEvent);
		allCoincidents.push_back(oneCoincidence);
		
	}
	for (int i=0;i<10;i++){
		cout<<"x"<<endl;
		cout<<allCoincidents[i][0].station<<endl;
	}
	
}
	
	
	
	
	
	
	
	
	
	
