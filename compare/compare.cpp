/*                   Stand 18.03.2018
	compare sortiert und vergleicht die Zeilen mehrerer Textdateien,
	indem es zeitlich korrelierte Eintraege erfasst
	und zusammen mit den Zeitdifferenzen in eine Datei schreibt
	Ziel: mehr als 2 Eingabe-Dateien (mehrere RasPi-Stationen) -> erfuellt
	Ziel: Einstellbarkeit der match-Kriterien (ab wann gelten  -> erfuellt
	Einträge als Zeitlich korreliert?) und versehen der
	Coincidents mit einem Guetefaktor  -> momentan Guetefaktor== Anzahl Coincidents (an einem Zeitpunkt)  -> erfuellt
*/
//  Geschrieben von <Marvin.Peter@physik.uni-giessen.de>, Teilelemente sind geklaut von <Lukas.Nies@physik.uni-giessen.de>

// On Linux compile with 'g++ -o Compare Compare.cpp -O' optional: '-fopenmp' flag
// Vielleicht kann man mit openmp noch etwas mehr optimieren durch multithreading aber eher nicht so viel
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
#include "../precise_time/precise_time.h"


struct doneVector
{
	int dateiNameIndex;
	int vector0Oder1;
};
using namespace std;

unsigned long int oldBalken = 0;
void prozentanzeige(unsigned long int prozent, string& ausgabe)
{
	int width = 50;
	int balken;    char a = '|';
	balken = prozent*width/100;
	if (balken>oldBalken)
	{
		string prozentbalken = "";
		stringstream convert;
		convert<<prozent;
		prozentbalken += "\r ";
		prozentbalken += ausgabe;
		prozentbalken += " |";
		if (prozent<100)
		{
			//sonst length_error
			prozentbalken += string(balken, a);
			prozentbalken += string(width-balken, ' ');
		}
		else
		{
			prozentbalken += string(width, a);
		}
		prozentbalken += "|";
		prozentbalken += " ";
		if (prozent<100)
		{
			prozentbalken += convert.str();
		}
		else {
			//sonst mit etwas pech ueber 100%
			prozentbalken += "100";
		}
		prozentbalken += "%";
		prozentbalken += "     ";
		cout<<prozentbalken<<flush;// <<endl;
		//cout<<"\r[%3d%%]"<<prozentbalken; 
		oldBalken = balken;
	}
}


bool areTimestampsInOrder(string dateiName, bool verbose)
{   //ueberpruefe ob die Timestamps in der Datei mit Dateinamen der als string uebergeben wird zeitlich geordnet sind
	ifstream dateiInputstream(dateiName.c_str());
	if (!dateiInputstream.good())
	{
		cout<<"failed to read from file (in check Timestamp order section of code)"<<endl;
		exit(EXIT_FAILURE);
	}
	precise_time oldValue(0.0);
	
	long unsigned int lineCounter = 0;
	bool isInOrder = true;
	string oneLine;
	while (!dateiInputstream.eof())
	{
		//skipvalues();
		lineCounter++;
		getline(dateiInputstream, oneLine);
		if (dateiInputstream.eof())
		{
			break;
		}
		
		precise_time newValue(oneLine);
		cout.precision(9);
		cout<<fixed;
		
		if (oldValue<=newValue)
		{
			oldValue = newValue;
			
		}
		else
		{
			if (verbose)
			{
				cout<<"Line "<<lineCounter<<" in "<<dateiName<<" is not in order: "<<endl;
				cout<<"..."<<endl<<oldValue<<endl<<newValue<<endl<<"..."<<endl<<endl;
			}
			isInOrder = false;
			oldValue = newValue;
		}
	}
	return isInOrder;
}//end of areTimestampsInOrder(string ...)
bool areTimestampsInOrder(string dateiName, bool verbose, unsigned long int& lineCounter)
{   //ueberpruefe ob die Timestamps in der Datei mit Dateinamen der als string uebergeben wird zeitlich geordnet sind
	ifstream dateiInputstream(dateiName.c_str());
	if (!dateiInputstream.good())
	{
		cout<<"failed to read from file (in check Timestamp order section of code)"<<endl;
		exit(EXIT_FAILURE);
	}
	precise_time newValue(0.0);
	precise_time oldValue(0.0);
	lineCounter = 0;
	bool isInOrder = true;
	string oneLine;
	while (!dateiInputstream.eof())
	{
		//skipvalues();
		lineCounter++;
		getline(dateiInputstream, oneLine);
		if (dateiInputstream.eof())
		{
			break;
		}
		
		precise_time newValue(oneLine);
		cout.precision(9);
		cout<<fixed;
		if (oldValue<=newValue)
		{
			oldValue = newValue;
		}
		else
		{
			if (verbose)
			{
				cout<<"Line "<<lineCounter<<" in "<<dateiName<<" is not in order: "<<endl;
				cout<<"..."<<endl<<oldValue<<endl<<newValue<<endl<<"..."<<endl<<endl;
			}
			isInOrder = false;
			oldValue = newValue;
		}
	}
	return isInOrder;
}//end of areTimestampsInOrder(string ...)

void skipColumn(istream& data, int col)
{
	for (int i = 0; i<col; ++i)
		data.ignore(numeric_limits<streamsize>::max(), ' '); // col Leerzeichen-getrennte Werte uberspringen
}

void readToVector(ifstream& inputstream, vector<precise_time>& oneVector, int& maxValues, int& column)
{
	// ueberschreibt einen Vektor mit neuen Werten aus der Datei (aus dem inputstream)
	// es werden maximal "maxValues" viele Eintraege hinzugefuegt,
	// es wird gestoppt falls keine weiteren Daten im InputFile sind
	oneVector.clear();
	string oneLine;

	for (unsigned int i = 0; i<maxValues; i++)
	{
		if (inputstream.eof())
		{
			break;
		}
		if (column>0)
		{
			skipColumn(inputstream, column);
		}
		getline(inputstream, oneLine);
		//cout<<oneLine<<endl;
		precise_time oneValue(oneLine);
		//cout<<str<<endl;
	
		//cout<<oneValue<<endl;;
		oneVector.push_back(oneValue);
	}
}// end of readToVector(..)

void Usage()
{
	cout<<endl;
	cout<<"Compare"<<endl;
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
	cout<<endl;
	cout<<"Geschrieben von <Marvin.Peter@physik.uni-giessen.de>"<<endl;
	cout<<"(Teilelemente sind übernommen von <Lukas.Nies@physik.uni-giessen.de>"<<endl;
	cout<<"---Stand 18.03.2018---Bugs inclusive"<<endl<<endl;
}//end of Usage()

void Algorithm(vector<unsigned int>& base,
	vector<vector<vector<precise_time> > >& values,
	precise_time& matchKriterium,
	ofstream& output,
	vector<int>& currentVector,
	doneVector& fertigerVector)
{
	precise_time t(0.0);
	/*
		Algorithmus:    - schaue welcher Eintrag am kleinsten ist
						- finde ein Match aus den anderen Vektoren (alle Werte durchgehen), Index merken!
						- nutze aus, dass keine Werte kleiner diesem Match fuer ein Match in Frage kommen
	*/
	bool dateiEmpty[base.size()];
	for (unsigned int i = 0; i<base.size(); i++)
	{
		if (values[i][currentVector[i]].size()==0)
		{
			//cout<<"datei "<<i<<" = EMPTY ; base.size() = "<<base.size()<<" ; values["<<i<<"]["<<currentVector[i]<<"].size() = 0 ; ";
			dateiEmpty[i]=true;
		}
		else {
			//cout<<"datei "<<i<<" not empty ; base.size() = "<<base.size()<<" ; values["<<i<<"]["<<currentVector[i]<<"].size() = "<<values[i][currentVector[i]].size();
			dateiEmpty[i]=false;
		}
	}
	//cout<<endl;
	bool done = false;
	while (!done)
	{
		//welcher hat den kleinsten Wert? (values und base haben die gleiche Laenge)
		//base enthaelt die aktuellen Indizes bei denen die einzelnen Vektoren gelesen werden sollen
		//base[j] und currentVector[j] gehoeren z.B. immer zu values[j] => values[j][currentVector[i]][base[j]] ist ein logisch korrekter sinnvoller Wert
		//fuer den aktuellen zu bearbeitenden Wert der j-ten Datei
		//currentVector[j] zeigt an ob der momentan zu bearbeitende Vektor 0 oder 1 ist (es gibt fuer jede Datei immer einen, der bearbeitet und einen der
		//vollgeschrieben wird zur selben Zeit)
		for (unsigned int i = 0; i<base.size(); i++)
		{
			if (dateiEmpty[i]==false) {
				if (base[i]>=values[i][currentVector[i]].size())
				{
					fertigerVector.dateiNameIndex = i;
					//cout<<"fertigerVector = ["<<i<<"]["<<currentVector[i]<<"] ";
					fertigerVector.vector0Oder1 = currentVector[i];
					return;
				}
			}
		}		
		unsigned int indexSmallest = 0;
		for (unsigned int i = 0; i<base.size(); i++)
		{
			if (dateiEmpty[indexSmallest]==true)
			{
				indexSmallest = indexSmallest+1;
				//cout<<" ; indexSmallest = "<<indexSmallest<<endl;
			}
		}
		if (indexSmallest>=base.size())
		{
			//cout<<"indexSmallest >= base.size()"<<endl;
			return;
		}
		for (unsigned int i = 0; i<base.size(); i++)
		{
			if (dateiEmpty[i]==false)
			{
				if ((values[i][currentVector[i]][base[i]]<values[indexSmallest][currentVector[indexSmallest]][base[indexSmallest]]))
				{
					indexSmallest = i;
				}
			}
		}
		int coincidents = 0;
		for (unsigned int i = 0; i<base.size(); i++)
		{
			//hier wird entschieden ob es ein CoincidenceEreignis ist und es wird gleich in die Datei geschrieben
			if (dateiEmpty[i]==false) {
				if ((indexSmallest!=i)&&(abs(values[i][currentVector[i]][base[i]]-values[indexSmallest][currentVector[indexSmallest]][base[indexSmallest]])<=matchKriterium))
				{
					if (coincidents<1)
					{
						//t = values[indexSmallest][currentVector[indexSmallest]][base[indexSmallest]];
						//struct tm *tm = localtime(&t);
						//char date[20];
						//strftime(date, sizeof(date), "%Y.%m.%d %H:%M:%S", tm);
						output<<"  "<<indexSmallest<<"  "<<values[indexSmallest][currentVector[indexSmallest]][base[indexSmallest]]<<"  ";
						coincidents++;
					}
					output<<i<<"-"<<indexSmallest<<"  "<<(values[i][currentVector[i]][base[i]]-values[indexSmallest][currentVector[indexSmallest]][base[indexSmallest]])<<"   ";
					coincidents++;
				}
			}
		}
		if (coincidents>0)
		{
			output<<coincidents<<endl;
		}
		base[indexSmallest]++;
	}
}//end of Algorithmus

int main(int argc, char*argv[])
{
	string outputDateiName;
	vector <string> dateiName;
	time_t start, end, readToVectorTime, checkTimestampOrderTime, algorithmTime;
	
	precise_time oneValue(0.0);
	int maxTimestampsAtOnce = 2000;

	// Einlesen der Zusatzinformationen aus der Konsole
	bool verbose = false;
	int ch;
	int column1 = 0;
	int column2 = 0;
	int b = 1000;
	bool notSorted = true;
	while ((ch = getopt(argc, argv, "svm:c:o:b:h?"))!=EOF)
	{
		switch ((char)ch)
		{
		case 's':  notSorted = false;
		case 'o':  outputDateiName = optarg;
			//printf("Output wurde gewaehlt. %s.c_str()\n", out.c_str());
			break;
		case 'h': Usage();
			return -1;
			break;
		case 'v': verbose = true;
			break;
		case 'b': b = atoi(optarg);
			break;
		case 'c': column1 = atoi(optarg); //momentan nicht individuell fuer jede Datei
			break;
			/*case 'C':column2 = atoi(optarg); momentan nicht moeglich
				break;*/
		case 'm': maxTimestampsAtOnce = atoi(optarg);
			break;
		}
	}
	int maxTimestampsInVector = maxTimestampsAtOnce/4;
	precise_time matchKriterium(0,b,1); //Umrechnen, damit man die Eingabe in [us] machen kann
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


	// Ueberpruefung der Dateien auf Reihenfolge
	cout<<endl<<"check files...."<<flush;
	if (verbose)
	{
		cout<<endl<<endl;
	}

	start = time(0);
	vector<unsigned long int> lines;
	if (notSorted)
	{
		for (unsigned int i = 0; i<dateiName.size(); i++)
		{
			lines.push_back(0);
			if (!areTimestampsInOrder(dateiName[i], verbose, lines[i]))
			{
				if (verbose)
				{
					// falls die Timestamps nicht in der richtigen Reihenfolge gespeichert sind,
					// werden sie automatisch mit Unix sort sortiert, ob dann auch gleich ueberschrieben
					// laesst sich einstellen, momentan ja
					cout<<"sort "<<dateiName[i]<<" with Unix sort"<<endl<<endl;
				}
				string systemOut = "sort -g ";
				systemOut += dateiName[i];
				systemOut += " > ";
				systemOut += "tmp";
				char *outs = &systemOut[0];
				system(outs);
				systemOut = "mv -v tmp ";
				systemOut += dateiName[i];
				outs = &systemOut[0];
				system(outs);
			}
		}
	}
	else {
		// lese oder schätze zeilenmenge!!!
		for (unsigned int i = 0; i<dateiName.size(); i++)
		{
			lines[i] = 10000;

		}
	}
	unsigned long int overallLines = 0;
	for (unsigned int i = 0; i<lines.size(); i++)
	{
		// zaehlt alle Zeilen in allen Dateien (schon waehrend der Reihenfolgen-Ueberpruefung
		// und addiert sie. Ist wichtig fuer den Ladebalken
		overallLines += lines[i];
	}
	end = time(0);
	checkTimestampOrderTime = end-start;
	if (verbose)
	{
		cout<<endl<<"Ueberpruefung auf reihenfolge abgeschlossen, Dauer: "<<checkTimestampOrderTime<<"s"<<endl;
	}

	// Erstellen der iostreams
	vector<ifstream*> data;
	for (size_t i = 0; i<dateiName.size(); i++)
	{
		ifstream* in = new ifstream;
		data.push_back(in);
		(*data[i]).open(dateiName[i].c_str());
		//data enthaelt alle ifstreams, einen fuer jede Datei
	}
	ofstream output(outputDateiName.c_str());
	output.precision(9);
	output<<fixed;

	//Prozentanzeige:
	unsigned long int prozent = 0;
	unsigned long int prozentCounter = 0;
	unsigned long int oldCounter = 0;
	string ausgabeAlgorithmus = "vergleiche...";
	//Prozentanzeige


	if (verbose)
	{
		cout<<endl<<"Starte nun den Algorithmus...."<<endl<<endl;
	}
	vector<vector<vector<precise_time> > > values;
	//values [index der Datei] [0 oder 1] [werte in long double]
	values.resize(dateiName.size());
	vector<unsigned int> base;
	doneVector fertigerVector;
	vector<int> currentVector;
	for (unsigned int i = 0; i<dateiName.size(); i++)
	{
		currentVector.push_back(0);
		base.push_back(0);
		values[i].resize(2);
		//hier werden die Vectors mit einer "festen Laenge" also Anzahl der Dateien gebaut
		//base, currentVector und der aeußerste Container von "values" sind alle korelliert mit dem "dateiName" vector,
		//haben also die gleiche Anzahl Eintraege wie es Dateien gibt
		//currentVector gibt an, ob gerade 0 oder 1 bei der jeweiligen Datei verarbeitet wird
		//base gibt an, welcher index in der jeweiligen Datei (an welcher stelle in Datei i gerade gelesen wird,
		//wird resettet, sobald von values0 auf values1 oder umgekehrt gewechselt wird.
	}
	fertigerVector.vector0Oder1 = 0;
	fertigerVector.dateiNameIndex = 0;
	start = time(0);
	prozentanzeige(0, ausgabeAlgorithmus);
	// alles mit '#pragma omp...' ist nur wichtig fuer openmp, ein multithreading-tool, was dem compiler vermittelt,
	// welche Zeilen parallel ausgefuehrt werden koennen

	for (unsigned int i = 0; i<dateiName.size(); i++)
	{
		readToVector(*data[i], values[i][0], maxTimestampsInVector, column1);
	}
	for (unsigned int i = 0; i<dateiName.size(); i++)
	{
		readToVector(*data[i], values[i][1], maxTimestampsInVector, column1);
	}
		Algorithm(base, values, matchKriterium, output, currentVector, fertigerVector);
	try
	{
		prozentCounter++;
		if (0.5*prozentCounter/maxTimestampsInVector>0.5*oldCounter/maxTimestampsInVector)
		{

			prozent = (100.0*prozentCounter*(maxTimestampsInVector/(long double)(overallLines)));

			prozentanzeige(prozent, ausgabeAlgorithmus);
			oldCounter = prozentCounter;
		}
	}
	catch (int e)
	{
		cout<<"berechnung von prozent falsch"<<endl;
		exit(EXIT_FAILURE);
	}
	//}
//} // bis hier hin: alle Vektoren vollschreiben und schon mal den ersten Vektor abarbeiten
// (und damit auch Teile anderer Vektoren)

// ab hier: Schleife wird so oft ausgefuehrt bis alle Dateien keine Eintraege mehr haben
// gleichzeitig Einlesen und bearbeiten im Optimalfall
// vom Algorithmus erhaelt man immer die Information, welcher Vektor als erstes abgearbeitet ist,
// dieser wird dann sofort ersetzt und mit diesem und den anderen Vektoren weitergemacht,
// der abgearbeitete Vektor wird neu vollgeschrieben mit Werten aus der Datei
	bool allDataEOF = false;
	while (!allDataEOF)
	{
		switch (fertigerVector.vector0Oder1)
		{
		case 0:
			base[fertigerVector.dateiNameIndex] = 0;
			currentVector[fertigerVector.dateiNameIndex] = 1;
			readToVector(*data[fertigerVector.dateiNameIndex], values[fertigerVector.dateiNameIndex][0], maxTimestampsInVector, column1);
			Algorithm(base, values, matchKriterium, output, currentVector, fertigerVector);
			break;
		case 1:
			base[fertigerVector.dateiNameIndex] = 0;
			currentVector[fertigerVector.dateiNameIndex] = 0;
			readToVector(*data[fertigerVector.dateiNameIndex], values[fertigerVector.dateiNameIndex][1], maxTimestampsInVector, column1);
			Algorithm(base, values, matchKriterium, output, currentVector, fertigerVector);
			break;
		default:
			cout<<"kennung des fertigen Vektors nicht korrekt!! shutdown"<<endl;
			exit(EXIT_FAILURE);
			break;
		}
		try
		{
			prozentCounter++;
			if (0.5*prozentCounter/maxTimestampsInVector>0.5*oldCounter/maxTimestampsInVector)
			{

				prozent = (100.0*prozentCounter*(maxTimestampsInVector/(long double)(overallLines)));
				prozentanzeige(prozent, ausgabeAlgorithmus);
				oldCounter = prozentCounter;
			}
		}
		catch (int e)
		{
			cout<<"berechnung von prozent falsch, prozentCounter "<<prozentCounter<<endl;
			exit(EXIT_FAILURE);

		}
		allDataEOF = true;
		for (unsigned int i = 0; i<data.size(); i++)
		{
			if (!(*data[i]).eof())
			{
				allDataEOF = false;
			}
		}
	} //end of while
	// ab hier: letzte reste die noch in den Vektoren sind, werden abgearbeitet
	for (int i = 0; i<dateiName.size()*2; i++)
	{
		switch (fertigerVector.vector0Oder1)
		{
		case 0:
			base[fertigerVector.dateiNameIndex] = 0;
			currentVector[fertigerVector.dateiNameIndex] = 1;
			break;
		case 1:
			base[fertigerVector.dateiNameIndex] = 0;
			currentVector[fertigerVector.dateiNameIndex] = 0;
			readToVector(*data[fertigerVector.dateiNameIndex], values[fertigerVector.dateiNameIndex][1], maxTimestampsInVector, column1);
			Algorithm(base, values, matchKriterium, output, currentVector, fertigerVector);
			break;
		default:
			cout<<"kennung des fertigen Vektors nicht korrekt!! shutdown"<<endl;
			exit(EXIT_FAILURE);
			break;
		}
		try
		{
			prozentCounter++;
			if (0.5*prozentCounter/maxTimestampsInVector>0.5*oldCounter/maxTimestampsInVector)
			{

				prozent = (100.0*prozentCounter*(maxTimestampsInVector/(long double)(overallLines)));
				prozentanzeige(prozent, ausgabeAlgorithmus);
				oldCounter = prozentCounter;
			}
		}
		catch (int e)
		{
			cout<<"berechnung von prozent falsch, prozentCounter "<<prozentCounter<<endl;
			exit(EXIT_FAILURE);

		}
	}//end of for  
	prozentanzeige(100, ausgabeAlgorithmus);
	cout<<endl<<endl;
	end = time(0);
	algorithmTime = end-start;
	if (verbose)
	{
		cout<<"Das Programm ist fertig, Dauer: "<<algorithmTime<<"s"<<endl;
	}
}//end of int main()
