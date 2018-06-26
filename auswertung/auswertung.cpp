
#include <iterator>
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
#include <limits.h>
#include "../precise_time/precise_time.h"

using namespace std;

struct onOff{
	
	
	precise_time time;
	int j;
	int source;
	
	};

struct state{
	
	precise_time time;
	vector<bool> s;
	
	};

void Usage()
{
	cout<<endl;
	cout<<"Compare_v3"<<endl;
	cout<<"Sortiert <File1>, ... <File n> falls nicht schon sortiert."<<endl;
	cout<<"Vergleicht <File1>, ... <File n> jeweils miteinander und sucht nach sog. coincidents"<<endl;
	cout<<"Timestamps und bildet aus beiden die Differenz. Das Ergebnis wird in"<<endl;
	cout<<"der Form:"<<endl;
	cout<<"[%Y.%m.%d %H:%M:%S] ... [Differenz1] ... [Differenz n] ... [coincidents]"<<endl;
	cout<<"in <output> gespeichert. <File1> und <File2> sollte die Form haben:"<<endl;
	cout<<"[Timestamp]		"<<endl<<endl;
	cout<<"Benutzung:    "<<"compare"<<" [-vh?][-o<output> -b<Bereich>] File1 File2"<<endl;
	cout<<"		Optionen:"<<endl;
	cout<<"		-h?		  : Zeigt diese Hilfeseite an."<<endl;
	cout<<"		-c 		  : Zeile, in der die Daten stehen (Standard Zeile 0). momentan noch nicht individuell fuer jede Datei"<<endl;
	cout<<"		-v		  : Steigert das Verbosity-Level"<<endl;
	cout<<"		-o <output>	  : Pfad/Name der Output-Datei."<<endl;
	cout<<"		-n <Stationenzahl>	  : Wahl der Zahl der Stationen. (Standard 2)"<<endl;
	cout<<"     -m <maxWerte>     : Wahl der gleichzeitig in Vektoren eingelesenen Timestamps"<<endl;
	cout<<"     -C <maxCoincidents>  : Wahl der Hoechstzahl von Koinzidenzen (fuer Debugging)"<<endl;
	cout<<endl;
	

}//end of Usage()



vector<string> splitString(string str, char d){
	
	cerr<<"a";
	vector<string> result;
	
	int position=0, space=0;
	cerr<<"c";
	int l=str.size();
	cerr<<l;
	while(position<l){
		//cerr<<"b";
		space = str.find(d, position+1);
		string substr = str.substr(position, space-position);
		result.push_back(substr);
		}
	return result;
	
	}


void skipColumn(istream& data, int col)
{
	for (int i = 0; i<col; ++i)
		data.ignore(numeric_limits<streamsize>::max(), ' '); // col Leerzeichen-getrennte Werte uberspringen
}




void readToVector(ifstream& inputstream, vector<onOff>& oneVector, int& maxValues, int& column, bool verbose)
{	if (verbose){cout<<"XreadToVector aufgerufen"<<endl;}

	cerr<<"F";
	oneVector.clear();
	string oneLine;
cerr<<"E";
	for (unsigned int i = 0; i<maxValues; i++)
	{
		if (inputstream.eof())
		{	if (verbose){cout<<"Datei fertig"<<endl;}
			break;
		}
		if (column>0)
		{
			skipColumn(inputstream, column);
		}
		cerr<<"D";
		getline(inputstream, oneLine);
		stringstream str(oneLine);
		
		vector<string> results;
		try{
			cerr<<oneLine.size();
	 results=splitString(oneLine, ' ');}
		catch(...){cerr<<"caught expetion"<<endl;
			continue;}
		
		
		precise_time oneTime(results[0]);
		int j, source;
			cerr<<"B";
		j=stoi(results[1]);
	cerr<<"C";
		source=stoi(results[2]);
		onOff oneOnOff={oneTime, j, source};
		if (verbose) {cerr<<"gelesen: "<<oneTime<<" "<<j<<" "<<source<<endl;}
		oneVector.push_back(oneOnOff);}
		
		
		
	
}// end of readToVector(..)







int main(int argc, char*argv[])
{
	string outputDateiName;
	string dateiName;
	time_t start, end, readToVectorTime, checkTimestampOrderTime, algorithmTime;

	precise_time oneValue();
	int maxTimestampsAtOnce = INT_MAX, coincidenceCounter=0;		//Bug: Bei ./compare pps.txt sig.txt -o output2000.txt -v -m 2000 fehlen am Ende 5 Minuten, 
	int maxCoincidents=INT_MAX, n=2;
	
	//die bei ./compare pps.txt sig.txt -o output.txt -v da sind



	// Einlesen der Zusatzinformationen aus der Konsole
	bool verbose = false;		//muss zu false geaendert werden
	int ch;
	int column1 = 0;
	int column2 = 0;

	bool notSorted = true;
	
	cout << fixed << setprecision(9);		//gibt double auf 9 Nachkommastellen aus
	
	//Lesen der Argumente von der Kommandozeile
	while ((ch = getopt(argc, argv, "vc:o:n:C:h?"))!=EOF)
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
		case 'n': n = atoi(optarg); 
			break;
		
		case 'c': column1 = atoi(optarg); //momentan nicht individuell fuer jede Datei
			break;
			/*case 'C':column2 = atoi(optarg); momentan nicht moeglich
				break;*/
		
			
		case 'C':maxCoincidents=atoi(optarg);
				if(verbose){cout<<"Hoechstens: "<<maxCoincidents<<endl;}
			break;
			

		}
	}
	
	
int maxTimestampsInVector = maxTimestampsAtOnce/4;
	


	
	//matchKriterium=1.1;		//debugging

	if (verbose)
	{
		cout<<"Programm gestarted...."<<endl;
		cout<<endl<<"lese maximal "<<maxTimestampsAtOnce<<" Werte gleichzeitig ein"<<endl<<endl;
	}
	int index = 0;
	
	//Liest die Dateinamen. Da das beliebig viele sein können, wird das nicht oben abgearbeitet
	
	
		//
	
		string temp = argv[0];
		dateiName = "sortedtest.txt";
		if (verbose)
		{
			cout<<index<<". Dateiname ist "<<dateiName.c_str()<<"."<<endl;
		}
		
	

	


	// Erstellen der iostreams
	ifstream inputstream;

	

		inputstream.open(dateiName.c_str());
		//inputstreams enthaelt alle ifstreams, einen fuer jede Datei
	
	
	
	
	
	
	ofstream output(outputDateiName.c_str());
	output.precision(9);
	output<<fixed;




	



vector<onOff> liste;
	for (unsigned int i = 0; i<dateiName.size(); i++)		//alle Dateien werden durchgegangen und ihr Inhalt gelesen
	{	if(verbose){cout<<i<<". Datei wird gelesen"<<endl;}
		readToVector(inputstream, liste, maxTimestampsInVector, column1, verbose);
	}

vector<state> S;
vector<bool> b;


for (unsigned int i = 0; i<n; i++)	
{b.push_back(false);}

	for (unsigned int i = 0; i<liste.size(); i++)		//alle Dateien werden durchgegangen und ihr Inhalt gelesen
	{	
		b[liste[i].source]=liste[i].j;
		
		state s={liste[i].time, b};
		S.push_back(s);
	}
	

	
	ofstream outstream;
	 outstream.open (outputDateiName );

  
	for(unsigned int i=0; i<S.size(); i++){
		
		string boolString="";
		
		for(unsigned int j=0; j<S[i].s.size(); j++){
			
			
			if(S[i].s[j]){
			boolString.append("true");}
			else{	boolString.append("false");}
			boolString.append("|");
			}
		
		outstream<<S[i].time<<" "<<boolString<<endl;
		
		
		
		}
	outstream.close();
	
	
}
