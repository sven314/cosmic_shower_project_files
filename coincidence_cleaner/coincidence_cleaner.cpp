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
			
			
			
		if(allCoincidents[tested][testedLength-k].timestamp-allCoincidents[compared][comparedLength-k].timestamp>0.1){
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
	

	long double match=2.5;


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
		if (dateiInputstream.eof())//Wenn fertig
		{
			break;
		}
	
		oneCoincidence.clear();
		
		getline(dateiInputstream, oneLine);
		if(oneLine.empty()){continue;}		//ignore empty lines
	
	
			//Suche Spaces zum Zerschneiden der Zeilen
			int space0=0;
			int space1=oneLine.find(" ",space0+1);
			int space2=oneLine.find(" ",space1+1);
			string stationstring, timestring;
			int oneStation, oneTimestamp;
			
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
			
			timestring=oneLine.substr(space1,space2);
			stringstream str2(timestring);
			str2>>oneTimestamp;
			newEvent.timestamp=oneTimestamp;
			
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
	

	
	//Ausgabe um Lesen zu pruefen
	
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
	bool includedCoincidents[allCoincidents.size()];
	
	for(int i=0; i<allCoincidents.size();i++){		//in includedCoincidents  wird vermerkt, ob eine Koinzidenz in einer anderen enthalten ist.
		includedCoincidents[i]=false;	//initialisiere mit false
		}
	
	for(int i=0;i<allCoincidents.size();i++) {
		if(verbose){cout<<"i: "<<i<<endl;}
		for(int j=i+1; j<allCoincidents.size();j++){		//man muss nur maximal bis zum Ende der Liste suchen und faengt eine Zeile unter dem zu Pruefenden an
			
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
			output<<allCoincidents[i][j].timestamp<<"    ";}
			
		
		
		output<<endl;
		
		}
	}
}
	

	
	
	
	
	
	
	
	
	
	
