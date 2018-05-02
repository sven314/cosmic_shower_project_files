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

#include <iostream>     // std::cout
#include <algorithm>   	//std::sort
#include <ctime>
#include "../precise_time/precise_time.h"

using namespace std;

void Usage()
{cout<<"USAGE"<<endl;}
class event {
	
	precise_time timestamp;
	
	unsigned int station;
	
	
	

public: event(precise_time time, unsigned int nstation) : timestamp(time), station(nstation) //einfacher Konstruktor
        {
        }
	
	public: precise_time get_timestamp(){
		return timestamp;
		
		}
	public: unsigned int get_station(){
		
		return station;
		}
	
	};
	
/*	
bool operator <(const timespec& time1, const timespec& time2)
{
    if (time1.tv_sec == time2.tv_sec)
        return time1.tv_nsec < time2.tv_nsec;
    else
        return time1.tv_sec < time2.tv_sec;
}


bool operator >(const timespec& time1, const timespec& time2)
{
    if (time1.tv_sec == time2.tv_sec)
        return time1.tv_nsec > time2.tv_nsec;
    else
        return time1.tv_sec > time2.tv_sec;
}
timespec operator +(const timespec& time1, const timespec& time2)
{
    struct timespec;
    
    timespec.tv_sec=time1.tv_sec+time2.tv_sec+(time1.tv_nsec+time2.tv_nsec)%1000000000;
}

*/



struct EventCompare  //https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects ermoeglicht das Sortieren von Vectotren aus Vektoren aus Events, 
//definiert, was groesser ist, sortiert aufsteigend nach  Timestamp

{
    inline bool operator() ( vector<event> & coinc1, vector<event> & coinc2)
    {	precise_time a=coinc1[0].get_timestamp(), b=coinc2[0].get_timestamp();
        return (a< b);
    }
};
	
	
//Prueft, ob tested in compared enthalten ist
bool included(vector<vector<event> > allCoincidents, int tested, int compared, bool verbose){	
	if(verbose){cout<<"included gestartet"<<endl; }
	
	
	
	int testedLength=allCoincidents[tested].size(), comparedLength=allCoincidents[compared].size();
	if(verbose){cout<<"testedLength "<<testedLength<<"ComparedLength "<<comparedLength<<endl;}
	//if(comparedLength<testedLength){return false;}		//Wenn es zu lang ist, passt es nicht rein
	
	for(int k=1; testedLength-k>=0&&comparedLength-k>=0;k++){
		
	
		
		if (verbose){
			cout<<"vergleiche "<<allCoincidents[tested][testedLength-k].get_station()<<" "<<allCoincidents[tested][testedLength-k].get_timestamp()<<" mit "<<allCoincidents[compared][comparedLength-k].get_station()<<" "<<allCoincidents[compared][comparedLength-k].get_timestamp()<<endl;
			cout<<"k: "<<k<<endl;
			}
	
		if(allCoincidents[tested][testedLength-k].get_station()!=allCoincidents[compared][comparedLength-k].get_station()){
			
			
			
			if(verbose){cout<<"stationenUnterschied"<<endl;}
			return false;
			
			}		//Wenn sie sich irgendwo unterscheiden, muss man nicht weiter pruefen, also return
			
			
			
		if((allCoincidents[tested][testedLength-k].get_timestamp()-allCoincidents[compared][comparedLength-k].get_timestamp())!=0){
			if(verbose){cout<<"timestampUnterschied"<<endl;}
			
			
			return false;
			}

		
	
		if(verbose){cout<<"Kein Unterschied gefunden"<<endl;}
		
		
		}
	return true;		//Wenn kein Unterschied gefunden wird, ist es enthalten
	
	
	}

int main(int argc, char*argv[])
{	cout<<"coincidence_cleaner started"<<endl<<endl;
	string outputDateiName="cleaned_out.txt";
	vector <string> dateiName;
	

	
	int maxCoincidentsAtOnce = INT_MAX;	
	int maxCoincidents=INT_MAX;
	

	precise_time match(2.5);



	// Einlesen der Zusatzinformationen aus der Konsole
	bool verbose = false;	
	int ch;

	

	
	cout << fixed << setprecision(9);		//gibt double auf 9 Nachkommastellen aus
	
	//Lesen der Argumente von der Kommandozeile
	while ((ch = getopt(argc, argv, "svm:c:o:h?"))!=EOF)
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

			break;
		
	

	
		case 'm': maxCoincidentsAtOnce = atoi(optarg);
				if (verbose) {cout<<"Es werden: "<<maxCoincidentsAtOnce<<" verarbeitet"<<endl;}
			break;
			
	
			

		}
	}	
	
	int k=0;
	if (argc-optind<1)
	{
		perror("Falsche Eingabe, zu wenige Argumente!\n");
		Usage();
		return -1;
	}
	for (int i = optind; i<argc; i++)
	{
		//
		dateiName.push_back("");
		string temp = argv[i];
		dateiName[k] = temp;
		if (verbose)
		{
			cout<<k<<". Dateiname ist "<<dateiName[k].c_str()<<"."<<endl;
		}
		k++;
	}

	ifstream dateiInputstream(dateiName[0].c_str());
	string oneLine;
	vector<event>  oneCoincidence;			//Eine kKoinzidenz ist eine Vector von Events
	vector<vector<event> > allCoincidents;	//	Alle Koinzidenzen als Vektor aus Vektoren aus Events
	
	//Lies
	for (unsigned int i = 0; i<maxCoincidentsAtOnce; i++)	{
		if(verbose) {cout<<"neue Zeile"<<endl;}
		if (dateiInputstream.eof())  //Wenn fertig
		{
			break;
		}
	
		oneCoincidence.clear();
		
		getline(dateiInputstream, oneLine);
		if(oneLine.empty()){continue;}		//ignore empty lines
	
			if(verbose){cout<<oneLine<<endl;}
			//Suche Spaces zum Zerschneiden der Zeilen
			int space0=0;
			int space1=oneLine.find(" ",space0+1);
			int space2=oneLine.find(" ",space1+1);
			//int dec_div=oneLine.find(".",space1+1);
			string stationstring, timestring;//timestring_sec, timestring_nsec;
			int oneStation;
			//long double oneTimestamp_sec, oneTimestamp_nsec;
			int counter=0;
			
		//Liest eine Zeile
		while(true)	{
	
			
			if(verbose){cout<<"new Timestamp"<<endl;}
			try{
			
		
			
			stationstring=oneLine.substr(space0,space1);
			stringstream str1(stationstring);
			str1>>oneStation;
		
			
			precise_time newTime(oneLine.substr(space1,space2));
			
			
			
			event newEvent(newTime, oneStation);
		
			if(verbose){cout<<newEvent.get_station()<<" ";}
			
	
			oneCoincidence.push_back(newEvent);
			
			if(verbose){cout<<oneCoincidence[counter].get_station()<<endl;}
			//Schiebe die spaces 0--2, 1 hinter 2 und 2 hinter neue 1
			space0=space2;
			space1=oneLine.find(" ",space2+1);
			space2=oneLine.find(" ",space1+1);
			counter++;}
			
			//Wenn eine Zeile zu Ende ist, gibt es eine Exception.
			catch (exception E){
				if(verbose){cout<<"caught out of range"<<endl;}
				break;
				}
		 }
		 
		 allCoincidents.push_back(oneCoincidence);
	}
	
	//Sortieren
	
	
	sort (allCoincidents.begin(), allCoincidents.end(), EventCompare());
	
	//Ausgabe um Lesen und Sortieren zu pruefen
	
	cout<<"Zeilenzahl: "<<allCoincidents.size()<<endl;
	if(verbose){cout<<"Ausgabe beginnt"<<endl;}
	for (int i=0;i<allCoincidents.size();i++){		// j innerhalb einer Coincidednmce, i fuer die Zeile  
		
		for(int j=0; j<allCoincidents[i].size(); j++){
			
			if(verbose){
				
				cout<<allCoincidents[i][j].get_station()<<": ";
				cout<<allCoincidents[i][j].get_timestamp()<<"    ";}
			
		}
		
		if(verbose){cout<<endl;}
	}
	
	//Beginn der eigentlichen Pruefung
	bool includedCoincidents[allCoincidents.size()];
	
	for(int i=0; i<allCoincidents.size();i++){		//in includedCoincidents  wird vermerkt, ob eine Koinzidenz in einer anderen enthalten ist.
		includedCoincidents[i]=false;	//initialisiere mit false
		}
	
	for(int i=allCoincidents.size()-1;i>=0;i--) {
		if(verbose){cout<<"i: "<<i<<endl;}
		for(int j=i-1; j>=0;j--){		//man muss nur maximal bis zum Anfang der Liste suchen und faengt eine Zeile ueber dem zu Pruefenden an
			
			if(included(allCoincidents, i, j, verbose)){
			includedCoincidents[i]=true;		
			
			if(verbose){cout<<"enthalten"<<endl;}
			break;
			}
				
				
			if(verbose){cout<<"j: "<<j<<" "<<endl;}
		
			if(allCoincidents[i][0].get_timestamp()-allCoincidents[j][0].get_timestamp()>match){
				break;}		//Suche nicht mehr weiter, wenn die Zeit-Abstaende zu gross werden
			
			
			
			}
			
		
		if(verbose){cout<<endl;}
		}
	cout<<"Ergebnis"<<endl;	
	

		
	ofstream output(outputDateiName.c_str());
	
	output << fixed << setprecision(9);
	
	
	int discarded=0;
	
	//Zusammenfassung auf Konsole schreiben
	for(int i=0; i<allCoincidents.size();i++){
		cout<<"Zeile: "<<i;
		if(includedCoincidents[i]){
			discarded++;
			cout<<" verworfen"<<endl;
			
			
			
			}else{
				cout<<" behalten"<<endl;}
		
		}
	
	cout<<discarded<<" Zeilen verworfen"<<endl;
	
	
	//Gefilterte Daten in outputfile schreiben.	
	for (int i=0;i<allCoincidents.size();i++){		// j innerhalb einer Coincidednmce, i fuer die Zeile  
		
		if(!includedCoincidents[i]){
		
		for(int j=0; j<allCoincidents[i].size(); j++){
	
			
			output<<allCoincidents[i][j].get_station()<<": ";
			output<<allCoincidents[i][j].get_timestamp()<<"    ";
			output<<allCoincidents[i][j].get_timestamp()<<"    ";}
			
		
		
		output<<endl;
		
		}
	}
}
	

	
	
	
	
	
	
	
	
	
	
