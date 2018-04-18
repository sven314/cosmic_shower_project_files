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


using namespace std;

void Usage()
{cout<<"USAGE"<<endl;}
struct Event {
	
	struct timespec timestamp;
	
	unsigned int station;
	
	
	
	};

struct EventCompare  //https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects ermoeglicht das Sortieren von Vectotren aus Vektoren aus Events, 
//definiert, was groesser ist, sortiert aufsteigend nach  Timestamp

{
    inline bool operator() (const vector<Event> & coinc1, const vector<Event> & coinc2)
    {
        return (coinc1[0].timestamp < coinc2[0].timestamp);
    }
};
	
	
//Prueft, ob tested in compared enthalten ist
bool included(vector<vector<Event> > allCoincidents, int tested, int compared, bool verbose){	
	if(verbose){cout<<"included gestartet"<<endl; }
	
	
	
	int testedLength=allCoincidents[tested].size(), comparedLength=allCoincidents[compared].size();
	if(verbose){cout<<"testedLength "<<testedLength<<"ComparedLength "<<comparedLength<<endl;}
	//if(comparedLength<testedLength){return false;}		//Wenn es zu lang ist, passt es nicht rein
	
	for(int k=1; testedLength-k>=0&&comparedLength-k>=0;k++){
		
	
		
		if (verbose){
			cout<<"vergleiche "<<allCoincidents[tested][testedLength-k].station<<" "<<allCoincidents[tested][testedLength-k].timestamp<<" mit "<<allCoincidents[compared][comparedLength-k].station<<" "<<allCoincidents[compared][comparedLength-k].timestamp<<endl;
			cout<<"k: "<<k<<endl;
			}
	
		if(allCoincidents[tested][testedLength-k].station!=allCoincidents[compared][comparedLength-k].station){
			
			
			
			if(verbose){cout<<"stationenUnterschied"<<endl;}
			return false;
			
			}		//Wenn sie sich irgendwo unterscheiden, muss man nicht weiter pruefen, also return
			
			
			
		if(allCoincidents[tested][testedLength-k].timestamp-allCoincidents[compared][comparedLength-k].timestampInt!=0){
			if(verbose){cout<<"timestampUnterschied"<<endl;}
			
			
			return false;
			}

		
	
		if(verbose){cout<<"Kein Unterschied gefunden"<<endl;}
		
		
		}
	return true;		//Wenn kein Unterschied gefunden wird, ist es enthalten
	
	
	}

int main(int argc, char*argv[])
{
	string outputDateiName="cleaned_out.txt";
	vector <string> dateiName;
	

	
	int maxCoincidentsAtOnce = INT_MAX;	
	int maxCoincidents=INT_MAX;
	

	timespec match;
	match.tv_sec=2;
	match.tv_nsec=500000000;


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
	vector<Event>  oneCoincidence;			//Eine kKoinzidenz ist eine Vector von Events
	vector<vector<Event> > allCoincidents;	//	Alle Koinzidenzen als Vektor aus Vektoren aus Events
	
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
			int dec_div=oneLine.find(".",space1+1);
			string stationstring, timestring_sec, timestring_nsec;
			int oneStation;
			long double oneTimestamp_sec, oneTimestamp_nsec;
			int counter=0;
			
		//Liest eine Zeile
		while(true)	{
	
			
			if(verbose){cout<<"new Timestamp"<<endl;}
			try{
			
			Event newEvent;
			
			stationstring=oneLine.substr(space0,space1);
			stringstream str1(stationstring);
			str1>>oneStation;
			newEvent.station=oneStation;
			
			timestring_sec=oneLine.substr(space1,dec_div);
			if(verbose){cout<<timestring_sec<<endl;}		//Hier sind die Nachkommastellen noch da
			stringstream str2(timestring_sec);
			str2>>oneTimestamp_sec;
			if(verbose){cout<<oneTimestamp_sec<<endl;}		//Hier nicht mehr
			newEvent.timestamp.tv_sec=oneTimestamp;
			
			timestring_nsec=oneLine.substr(dec_div,space2);
			if(verbose){cout<<timestring_nsec<<endl;}		//Hier sind die Nachkommastellen noch da
			stringstream str2(timestring_nsec);
			str2>>oneTimestamp_nsec;
			if(verbose){cout<<oneTimestamp_nsec<<endl;}		//Hier nicht mehr
			newEvent.timestamp.tv_nsec=oneTimestamp;
			
			if(verbose){cout<<newEvent.station<<" ";}
			
	
			oneCoincidence.push_back(newEvent);
			
			if(verbose){cout<<oneCoincidence[counter].station<<endl;}
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
				
				cout<<allCoincidents[i][j].station<<": ";
				cout<<allCoincidents[i][j].timestamp<<"    ";}
			
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
		
			if(allCoincidents[i][0].timestamp-allCoincidents[j][0].timestamp>match){
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
	
			
			output<<allCoincidents[i][j].station<<": ";
			output<<allCoincidents[i][j].timestamp.tv_sec<<"    ";
			output<<allCoincidents[i][j].timestamp.tv_nsec<<"    ";}
			
		
		
		output<<endl;
		
		}
	}
}
	

	
	
	
	
	
	
	
	
	
	
