/*                   Stand 18.10.2017
	compare_v3 sortiert und vergleicht die Zeilen mehrerer Textdateien,
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


/*Bugs?
 * 
 * Hier wird 2-2 nicht gefunden
 * 	1	3	3
 *  2	2	4
 *  2,5 3	3
 *      3,1 5
 *      5   6
 * 
 * 
 * 
 * 
 * 
 * Wenn eins fertig ist, so lange die anderen mit last value vergleichen bis diese alle einen groesseren Abstand zu last value haben als dsas matchkriterium (Das koennen bei dinnvollen matchkriterien nicht viele sein. (Was passiert, wenn dabei eine Datei fertig wird?) Dann weiter als gaebe es die fetige Datei gar nicht
 */
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


using namespace std;

bool areTimestampsInOrder(string dateiName, bool verbose)
{   //ueberpruefe ob die Timestamps in der Datei mit Dateinamen, der als string uebergeben wird, zeitlich geordnet sind
	ifstream dateiInputstream(dateiName.c_str());
	if (!dateiInputstream.good())
	{
		cout<<"failed to read from file (in check Timestamp order section of code)"<<endl;
		exit(EXIT_FAILURE);
	}
	long double oldValue = 0.0;
	long double newValue = 0.0;
	long unsigned int lineCounter = 0;
	bool isInOrder = true;
	string oneLine;
	while (!dateiInputstream.eof())
	{
		//skipvalues();
		lineCounter++;
		getline(dateiInputstream, oneLine);
		stringstream str(oneLine);
		str>>newValue;
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
{   //ueberpruefe ob die Timestamps in der Datei mit Dateinamen, der als string uebergeben wird, zeitlich geordnet sind
	ifstream dateiInputstream(dateiName.c_str());
	if (!dateiInputstream.good())
	{
		cout<<"failed to read from file (in check Timestamp order section of code)"<<endl;
		exit(EXIT_FAILURE);
	}
	long double oldValue = 0.0;
	long double newValue = 0.0;
	lineCounter = 0;
	bool isInOrder = true;
	string oneLine;

	while (!dateiInputstream.eof())
	{
		//skipvalues();
		lineCounter++;
		getline(dateiInputstream, oneLine);
		stringstream str(oneLine);
		str>>newValue;
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

void readToVector(ifstream& inputstream, vector<long double>& oneVector, int& maxValues, int& column, bool verbose)
{	if (verbose){cout<<"readToVector aufgerufen"<<endl;}
	// ueberschreibt einen Vektor mit neuen Werten aus der Datei (aus dem inputstream)
	// es werden maximal "maxValues" viele Eintraege hinzugefuegt,
	// es wird gestoppt falls keine weiteren Daten im InputFile sind
	oneVector.clear();
	string oneLine;
	long double oneValue;
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
		str>>oneValue;
		if (verbose) {cout<<"gelesen: "<<oneValue<<"..."<<endl;}
		oneVector.push_back(oneValue);
	}
}// end of readToVector(..)

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
	cout<<"Geschrieben von <Marvin.Peter@physik.uni-giessen.de>"<<endl;
	cout<<"(Teilelemente sind geklaut von <Lukas.Nies@physik.uni-giessen.de>"<<endl;
	cout<<"---Stand 20.03.2017---Bugs inclusive"<<endl<<endl;
}//end of Usage()

 //Nimm den Wert bei dem eine Datei fertig war und vergleiche mit allen noch nicht betrachteten Werten de
//r anderen Dateien bis man dort mehr als das Matchkroiterium vom betrachteten Wet weg ist

void writeMatch(bool verbose, long double firstTimestamp, unsigned int firstFile, long double secondTimestamp, unsigned int secondFile, long unsigned int coincidenceCounter, ofstream& output, char callingFunction){
	
	
						output<<firstTimestamp<<"(Aus Datei Nr. "<< firstFile<< ") und "
						<< secondTimestamp<<" (Aus Datei Nr. "<< secondFile<<") Sind Der "
						<< coincidenceCounter<<". Match. Ihr Abstand betraegt: "<<secondTimestamp-firstTimestamp<<" Herkunft: "<<callingFunction;
						
						
						if(verbose){cout<<firstTimestamp<<"(Aus Datei Nr. "<< firstFile<< ") und "
						<< secondTimestamp<<" (Aus Datei Nr. "<< secondFile<<") Sind Der "
						<< coincidenceCounter<<". Match. Ihr Abstand betraegt: "<<secondTimestamp-firstTimestamp<<" Herkunft: "<<callingFunction<<endl;}
						
						
	
	
	}





void processRemains(vector<unsigned int>& currentPosition,  
	vector<vector<long double> > & values,
	long double& matchKriterium,
	ofstream& output,

	int& fertigerVector, 
	bool& verbose,
	int& coincidenceCounter,
	vector<bool>& finished,
	vector<long double>& lastValues,
	int maxCoincidents) 
	
	
{	
	
	if(verbose){
		cout<<"Resteverarbeitung gestartet"<<endl;
		
		}
	int tempcurrentPosition[currentPosition.size()] ;
	
	for (unsigned int i=0; i<currentPosition.size();i++){		//erstelle Kopie von currentPosition, damit nach Ende von processEnd an der richtigen Stelle weitergemacht wird
		tempcurrentPosition[i]=currentPosition[i]+1;		//+1 Verhindert, dass schon gefundene nochmal gefunden werden (laesst hoffentlich nichts verschwinden(passt schon))
		
		}
		
	cout<<"Der letzte Wert in Datei "<<fertigerVector <<" ist "<<values[fertigerVector][currentPosition[fertigerVector]]<<endl;
	for (unsigned int i=0; i<currentPosition.size();i++){
		

	
		
		if (i==fertigerVector){continue;}		//Gehe zum naechsten i
		else{
			
				cout<<"Resteverwertung sucht Coincidents..."<<endl;
			
				
				while (fabs(values[i][tempcurrentPosition[i]]-values[fertigerVector][currentPosition[fertigerVector]])<=matchKriterium){
						//cout<<"...Gefunden ";
										
						coincidenceCounter++;
						
						if (coincidenceCounter>maxCoincidents){
			
							exit(EXIT_FAILURE);}
	
						//Alternatives Ausgabeformat
						
						
						writeMatch(verbose,values[fertigerVector][currentPosition[fertigerVector]], fertigerVector,values[i][tempcurrentPosition[i]], i, coincidenceCounter,  output, 'R' );
					
						
		
					tempcurrentPosition[i]++;
					
					}
					
			
				}
	}
	
	
	cout<<"...und ist fertig"<<endl;
	
	
}	
	

void endOfAlgorithm(time_t algorithmStart, int whiles, std::string reason) {	
	
	time_t algorithmEnd, algorithmDuration;
	
	
		
		algorithmEnd=time(0);
		algorithmDuration=algorithmEnd-algorithmStart;
		cout<<"Nach "<< whiles << " Durchlaeufen beendet, wegen: "<< reason <<"  Dauer: "<<algorithmDuration<<endl;
		
		
}

bool Algorithm(vector<unsigned int>& currentPosition,
	vector<vector<long double> > & values,
	long double& matchKriterium,
	ofstream& output,

	int& fertigerVector, 
	bool& verbose,
	int& coincidenceCounter,
	vector<bool>& finished,

	vector<long double>& lastValues,
	int maxCoincidents)
	
{	
	time_t t, algorithmStart, algorithmEnd, algorithmDuration;
	unsigned int whiles=0, noNotFinished=0; //Zaehler fuer Durchlaeufe der while(!done) Schleife
	
	/*
		Algorithmus:    - schaue welcher Eintrag am kleinsten ist
						- finde ein Match aus den anderen Vektoren (alle Werte durchgehen), Index merken!
						- nutze aus, dass keine Werte kleiner diesem Match fuer ein Match in Frage kommen
	*/
	
	if (verbose){
		cout<<"Der Algorithmus wurde aufgerufen"<<endl;
		algorithmStart=time(0);
		} 

	//cout<<endl;
	bool done = false;
	while (!done)
		{	whiles++;
			
		if(verbose&&(whiles<100||whiles%1000==0)){			
			
			cout << "while Aufgerufen. " <<whiles<<endl;
		}

		for (unsigned int i = 0; i<currentPosition.size(); i++)
		{
			if (finished[i]==false) {		//ueberspringe fertige Vektoren
				if (currentPosition[i]>=(values[i].size()-1))  //Wenn currentPosition[i] bis zur Zahl der Timestamps durchgelaufen ist, ist etwas (eine Datei) fertig.
				// Das wird für alle Dateien gemacht.
				{
					fertigerVector = i; 
					lastValues[i]=values[i][currentPosition[i]];
					finished[i]=true;   //Diese Datei ist durchlaufen worden
					
					cout<<"Gerade beendet: "<<i<<endl;
					
					
					
					
					noNotFinished=0;		//Zahl der noch nicht fertigen Dateien, muss mindestens 2 betragen, damit ein Weiterarbeiten sinnvoll ist
					
					for(unsigned int i=0; i<currentPosition.size(); i++){
						
						if (!finished[i]){
							noNotFinished++;
							
							}
						
						
						}
						
					if (noNotFinished>=1){
					
					processRemains(currentPosition, values, matchKriterium, output,  fertigerVector, verbose, coincidenceCounter, finished, lastValues, maxCoincidents);
					return false;		//starte Algorithmus neu

		}
		
				else {return true;}
					

				}
			}
		}	
		unsigned int indexSmallest;
		for (unsigned int i = 0; i<currentPosition.size(); i++)  {		//Beginne die Suche nach indexSmallest bei erster noch nicht fertiger Datei
			
			if(finished[i]==false){
				indexSmallest = i;
				break;
				}	
		}
	
		for (unsigned int i = 0; i<currentPosition.size(); i++)  // Alle nicht fertigen Dateien durchgehen, wenn leer ueberspringen,
		//sonst pruefen, ob der Wert bei values[i][currentVector[i]][currentPosition[i]] kleiner ist als der bei indexSmallest, wenn das der Fall ist, indexSmallest durch Index i ersetzen
		{
			if (finished[i]==false)
			{
				if ((values[i][currentPosition[i]]<values[indexSmallest][currentPosition[indexSmallest]]))
				{
					indexSmallest = i;
				}
			}
		}
		int coincidents = 1;
		for (unsigned int i = 0; i<currentPosition.size(); i++)   // Alle nicht fertigen Dateien durchgehen und jeweils mit dem bei indexSmallest vergleichen
		{
			if (finished[i]==false){
				
				
				//hier wird entschieden ob es ein CoincidenceEreignis ist und es wird gleich in die Datei geschrieben
	
				 {
					if ((indexSmallest!=i)&&(fabs(values[i][currentPosition[i]]-values[indexSmallest][currentPosition[indexSmallest]])<=matchKriterium))
					{
						
						coincidenceCounter++;
						coincidents++;
				
						//Alternatives Ausgabeformat
						
						writeMatch(verbose,values[indexSmallest][currentPosition[indexSmallest]], indexSmallest,values[i][currentPosition[i]], i, coincidenceCounter,  output, 'A' );
						
					/*	output<<values[indexSmallest][currentPosition[indexSmallest]]<<"(Aus Datei Nr. "<< indexSmallest<< ") und"
						<< values[i][currentPosition[i]]<<"(Aus Datei Nr. "<< i<<") Sind Der "
						<< coincidenceCounter<<". Match. Ihr Abstand betraegt: "<<(values[i][currentPosition[i]]-values[indexSmallest][currentPosition[indexSmallest]])<<" Herkunft A"<<endl;*/
						
					}
				}
			}
		}
		
		
		
		if (coincidents>1)
		{
			output<<" "<<coincidents<<"er Coincidence"<<endl;
		}
		currentPosition[indexSmallest]++;   //In der Datei, wo der kleinste Timestamp gefunden wurde, soll beim naechsten Durchlauf der darauf folgende genommen werden.
		done=true;
		for (unsigned int i=0; i<currentPosition.size(); i++)
	
				{
					if (!finished[i]){
					done=false;
				 
					}
	
				}
		}
		
		

	if (verbose==true) {
		
		endOfAlgorithm(algorithmStart, whiles , "durchgelaufen" );
		
		}
		
		
	if(done){return true;}		//ruefe Algorithm nicht weiter auf
	
}//end of Algorithmus



int main(int argc, char*argv[])
{
	string outputDateiName;
	vector <string> dateiName;
	time_t start, end, readToVectorTime, checkTimestampOrderTime, algorithmTime;
	long double matchKriterium;//in Sekunden
	long double oneValue;
	int maxTimestampsAtOnce = INT_MAX, coincidenceCounter=0;		//Bug: Bei ./compare pps.txt sig.txt -o output2000.txt -v -m 2000 fehlen am Ende 5 Minuten, 
	int maxCoincidents=INT_MAX;
	
	//die bei ./compare pps.txt sig.txt -o output.txt -v da sind



	// Einlesen der Zusatzinformationen aus der Konsole
	bool verbose = false;		//muss zu false geaendert werden
	int ch;
	int column1 = 0;
	int column2 = 0;
	int b = 100;
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
			return -1;
			break;
		
		case 'b': b = atoi(optarg);
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
	matchKriterium = b/1000000.0; //Umrechnen, damit man die Eingabe in [us] machen kann
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


	//Hier sollte  Ueberpruefung der Dateien auf Reihenfolge passieren


	start = time(0);	//Zur Berechnung der Prüfungsdauer der Reihenfolge
	vector<unsigned long int> lines;
/*	if (notSorted)
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
	}*/
//momentan unnoetig
	/*end = time(0);
	checkTimestampOrderTime = end-start;
	if (verbose)
	{
		cout<<endl<<"Ueberpruefung auf reihenfolge abgeschlossen, Dauer: "<<checkTimestampOrderTime<<"s"<<endl;
	}*/

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


	string ausgabeAlgorithmus = "vergleiche...";



	if (verbose)
	{
		cout<<endl<<"Starte nun den Algorithmus...."<<endl<<endl;
	}
	vector<vector<long double> > values;
	
	/* Struktur von Values:			Erster Index DateiNr zweiter index WertNr,  Wert Nr ergibt sich aus currentPosition 
	 * 
	 * Datei1	Datei2	etc...
	 * 
	 * Wert		Wert
	 * wert		Wert
	 * etc..	etc ..*/
	vector<bool> finished;		
	
	//Welche Dateien sind fertig?
	vector<long double> lastValues;		//Was war der letzte betrachtete Wert in einer fertigen Datei? 

	values.resize(dateiName.size());
	vector<unsigned int> currentPosition;
	int fertigerVector;
	//vector<int> currentVector;
	
	
	


		//hier werden die Vectors mit einer "festen Laenge" also Anzahl der Dateien gebaut
		//currentPosition, currentVector und der aeußerste Container von "values" sind alle korelliert mit dem "dateiName" vector,
		//haben also die gleiche Anzahl Eintraege wie es Dateien gibt

		//currentPosition gibt an, welcher index in der jeweiligen Datei (an welcher stelle in Datei i gerade gelesen wird,

	


	
	fertigerVector=0;
	start = time(0);
	if(verbose){
	cout<<"currentPosition.size: "<<currentPosition.size()<<endl;
}


	for (unsigned int i = 0; i<dateiName.size(); i++)		//alle Dateien werden durchgegangen und ihr Inhalt gelesen
	{	if(verbose){cout<<i<<". Datei wird gelesen"<<endl;}
		readToVector(*data[i], values[i], maxTimestampsInVector, column1, verbose);
	}
	
	
	for (unsigned int i = 0; i<dateiName.size(); i++)
	{
		
		currentPosition.push_back(0);		//Haenge an currentPosition eine 0 dran
		if (values[i].size()==0)		//leere Dateien werden wie abgearbeitete behandelt
		{
			
			finished.push_back(true);		//Finished gibt an ob eine bestimmte Datei schon fertig ist
			
			
		}else {
			
			
			finished.push_back(false);
		}
		
		
		lastValues.push_back(0);
	}	
	if(verbose){
		for (int i=0;i<dateiName.size(); i++){
			cout<<i<<": "<<finished[i]<<"; ";
			
			}
			
			cout<<endl;
			
	}
//	bool dateiEmpty[currentPosition.size()];
	
	bool algorithmFinished=false;
	while(!algorithmFinished){
		
		
	algorithmFinished=Algorithm(currentPosition, values, matchKriterium, output,  fertigerVector, verbose, coincidenceCounter, finished, lastValues, maxCoincidents);
	
	}
	
		


	if (verbose)
	{
		cout<<"Das Programm ist fertig, Dauer: "<<algorithmTime<<"s"<<endl;
	}
}
