

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
	cout<<"		-b <Bereich>	  : Wahl des Koinzidenten Bereiches in [us] (Standard: 100µs)."<<endl;
	cout<<"     -m <maxWerte>     : Wahl der gleichzeitig in Vektoren eingelesenen Timestamps"<<endl;
	cout<<"     -C <maxCoincidents>  : Wahl der Hoechstzahl von Koinzidenzen (fuer Debugging)"<<endl;
	cout<<endl;
	

}//end of Usage()


void skipColumn(istream& data, int col)
{
	for (int i = 0; i<col; ++i)
		data.ignore(numeric_limits<streamsize>::max(), ' '); // col Leerzeichen-getrennte Werte uberspringen
}




void readToVector(ifstream& inputstream, vector<precise_time>& oneVector, int& maxValues, int& column, bool verbose)
{	if (verbose){cout<<"readToVector aufgerufen"<<endl;}
	// ueberschreibt einen Vektor mit neuen Werten aus der Datei (aus dem inputstream)
	// es werden maximal "maxValues" viele Eintraege hinzugefuegt,
	// es wird gestoppt falls keine weiteren Daten im InputFile sind
	oneVector.clear();
	string oneLine;
	precise_time oneValue();
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
		getline(inputstream, oneLine);
		stringstream str(oneLine);
		precise_time oneValue(str.str());
		
		if (verbose) {cout<<"gelesen: "<<oneValue<<"..."<<endl;}
		oneVector.push_back(oneValue);
	}
}// end of readToVector(..)







int main(int argc, char*argv[])
{
	string outputDateiName;
	vector <string> dateiName;
	time_t start, end, readToVectorTime, checkTimestampOrderTime, algorithmTime;

	precise_time oneValue();
	int maxTimestampsAtOnce = INT_MAX, coincidenceCounter=0;		//Bug: Bei ./compare pps.txt sig.txt -o output2000.txt -v -m 2000 fehlen am Ende 5 Minuten, 
	int maxCoincidents=INT_MAX;
	
	//die bei ./compare pps.txt sig.txt -o output.txt -v da sind



	// Einlesen der Zusatzinformationen aus der Konsole
	bool verbose = false;		//muss zu false geaendert werden
	int ch;
	int column1 = 0;
	int column2 = 0;
	long long b;
	bool notSorted = true;
	
	cout << fixed << setprecision(9);		//gibt double auf 9 Nachkommastellen aus
	
	//Lesen der Argumente von der Kommandozeile
	while ((ch = getopt(argc, argv, "svm:c:o:b:C:h?"))!=EOF)
	{
		switch ((char)ch)
		{
		case 's':  notSorted = false;
		
		
		case 'v': verbose = true;
			break;
		case 'o':  outputDateiName = optarg;
			if (verbose){printf("Output wurde gewaehlt. %s \n", outputDateiName.c_str());}
			break;
		case 'h': Usage();
			return -1;
			
			break;
		
		case 'b': b = atof(optarg);		//MatchKriterium als long long in nanosekunden
			break;
		case 'c': column1 = atoi(optarg); //momentan nicht individuell fuer jede Datei
			break;
			/*case 'C':column2 = atoi(optarg); momentan nicht moeglich
				break;*/
		case 'm': maxTimestampsAtOnce = atoi(optarg);
				if (verbose) {cout<<"Es werden: "<<maxTimestampsAtOnce<<" verarbeitet"<<endl;}
			break;
			
		case 'C':maxCoincidents=atoi(optarg);
				if(verbose){cout<<"Hoechstens: "<<maxCoincidents<<endl;}
			break;
			

		}
	}
	
	
	int maxTimestampsInVector = maxTimestampsAtOnce/4;
	
	cout<<b<<endl;
	precise_time matchKriterium(0,b,1);
	
	//matchKriterium=1.1;		//debugging
	if (argc-optind<2)
	{
		perror("Falsche Eingabe, zu wenige Argumente!\n");
		Usage();
		return -1;
	}
	if (verbose)
	{
		cout<<"Programm gestarted...."<<endl;
		cout<<endl<<"lese maximal "<<maxTimestampsAtOnce<<" Werte gleichzeitig ein"<<endl<<endl;
	}
	int index = 0;
	
	//Liest die Dateinamen. Da das beliebig viele sein können, wird das nicht oben abgearbeitet
	for (int i = optind; i<argc; i++)
	{
		//
		dateiName.push_back("");
		string temp = argv[i];
		dateiName[index] = temp;
		if (verbose)
		{
			cout<<index<<". Dateiname ist "<<dateiName[index].c_str()<<"."<<endl;
		}
		index++;
	}

	cout<<"Das gewähltes MatchKriterium ist: "<<matchKriterium<<"s"<<endl;
	//Hier sollte  Ueberpruefung der Dateien auf Reihenfolge passieren


	// Erstellen der iostreams
	vector<ifstream*> inputstreams;
	for (size_t i = 0; i<dateiName.size(); i++)
	{
		ifstream* in = new ifstream;
		inputstreams.push_back(in);
		(*inputstreams[i]).open(dateiName[i].c_str());
		//inputstreams enthaelt alle ifstreams, einen fuer jede Datei
	}
	
	
	
	
	
	ofstream output(outputDateiName.c_str());
	output.precision(9);
	output<<fixed;

vector<vector<precise_time> > values;
	
	/* Struktur von Values:			Erster Index DateiNr zweiter index WertNr,  Wert Nr ergibt sich aus currentPosition 
	 * 
	 * Datei1	Datei2	etc...
	 * 
	 * Wert		Wert
	 * wert		Wert
	 * etc..	etc ..*/
	vector<bool> finished;		
	
	//Welche Dateien sind fertig?
		//Was war der letzte betrachtete Wert in einer fertigen Datei? 

	values.resize(dateiName.size());

	int fertigerVector;
	//vector<int> currentVector;
	
	
	


		//hier werden die Vectors mit einer "festen Laenge" also Anzahl der Dateien gebaut
		//currentPosition, currentVector und der aeußerste Container von "values" sind alle korelliert mit dem "dateiName" vector,
		//haben also die gleiche Anzahl Eintraege wie es Dateien gibt

		//currentPosition gibt an, welcher index in der jeweiligen Datei (an welcher stelle in Datei i gerade gelesen wird,

	


	



vector<onOff> liste;
	for (unsigned int i = 0; i<dateiName.size(); i++)		//alle Dateien werden durchgegangen und ihr Inhalt gelesen
	{	if(verbose){cout<<i<<". Datei wird gelesen"<<endl;}
		readToVector(*inputstreams[i], values[i], maxTimestampsInVector, column1, verbose);
	}


	
	
for (unsigned int i = 0; i<dateiName.size(); i++)	
	{	
		
		 for (unsigned int k = 0; k<values[i].size(); k++)	{
		
			 
		
			 onOff newOnOff1={0, 0,0};
			 newOnOff1.time=values[i][k]-matchKriterium;
			
			 newOnOff1.j=1;
			 newOnOff1.source=i;
			 liste.push_back(newOnOff1);
			 
			 onOff newOnOff2={0, 0,0};
			 newOnOff2.time=values[i][k]+matchKriterium;
		
			 newOnOff2.j=-1;
			 newOnOff2.source=i;
			 liste.push_back(newOnOff2);
			 
			 
			 }
		
		
		
	
	}
	
	ofstream outstream;
	 outstream.open (outputDateiName );

  
	for(unsigned int i=0; i<liste.size(); i++){
		
		outstream<<liste[i].time<<" "<<liste[i].j<<" "<<liste[i].source<<endl;
		
		
		
		}
	outstream.close();
	
	
}
