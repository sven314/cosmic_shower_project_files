
/*
 * 
 * 
 * 
 * 
 * 
 * Stand 06.10.2017
Fuer Test keine Timestamps ueber 50000 moeglich
*/

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <limits.h>

//Hiervon sind mache unnoetig
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


class event {
	 public: virtual ~event() {} 	//macht event polymorphic https://stackoverflow.com/questions/15114093/getting-source-type-is-not-polymorphic-when-trying-to-use-dynamic-cast
	public: long double timestamp;
	public: char eventType; //Falling 'f', rising 'r', Satelliten's'...(..), unkown 'u'

	
	};
class signalEvent : public event{
	public: int accuracy;
	public: long int eventCounter;
	
	
	};
	
class satelliteEvent : public event{
	public: int satellites;
	
	public: long double ago;
	};
	
//Hier noch Klassen fuer die anderen eventTypes einfuegen


void Usage(){		//(const char* progname)
	
	cout<<"++++++++++++Hilfe++++++++++++"<<endl;
	cout<<endl;
	cout<<"timestampQuality"<<endl<<endl;
	
	cout<<"Zweck:"<<endl;
	cout<<"Prueft Qualitaet von Timestamps, die vom ublox-Chip kommen anhand"<<endl;
	cout<<	"der Dauer zwischen rising und falling edge"<<endl;
	cout<< "Zaehlt verlorene Timestamp anhand von Luecken im Counter"<<endl<<endl;
	
	cout<<"Erwartetes Eingabeformat:"<<endl;
	cout<<"[long double timestamp] [char eventType r/f/s/t/q/n/a/b/p] ..."<<endl;
	cout<<"		(moegliche eventTypes r=rising, f=falling, s=Satelliten, ab hier noch nicht: t=time accuracy, q=quant error, n=noise, a=agc, b=TX buf usage, p=TX buf peak usage)"<<endl;
	cout<<"...[int accuracy] [long int Counter] (bei r und f)"<<endl;
	cout<<"...[int satellites] [long double ago] (bei s)"<<endl;
	cout<<"...und die anderen kommen noch"<<endl<<endl;
	
 


	cout<<"Benutzung:    "<<"timestampQuality"<<" [-vh?] -i inputfile -o outputfile"<<endl;
	cout<<"		Optionen:"<<endl;
	cout<<"		-h?		  : Zeigt diese Hilfeseite an."<<endl;
	//cout<<"		-c 		  : Zeile, in der die Daten stehen (Standard Zeile 0). momentan noch nicht individuell fuer jede Datei"<<endl;
	cout<<"		-v		  : Steigert das Verbosity-Level"<<endl;
	//cout<<"		-o <output>	  : Pfad/Name der Output-Datei."<<endl;
	cout<<"         -l <logfile>   : Pfad/Name der Log-Datei"<<endl;
	cout<<"         -m <logfile>   : Minimale Signaldauer in sec"<<endl;
	cout<<"         -M <logfile>   : Maximale Signaldauer in sec"<<endl;
	
	//cout<<"		-b <Bereich>	  : Wahl des Koinzidenten Bereiches in [us] (Standard: 100µs)."<<endl;
	//cout<<"     -m <maxWerte>     : Wahl der gleichzeitig in Vektoren eingelesenen Timestamps"<<endl;
	cout<<endl;
	cout<<"Geschrieben von <sven.peter@physik.uni-giessen.de> Teile aus CompareV3.cpp von <marvin.peter@physik.uni-giessen.de> uebernommen"<<endl;
	cout<<"Stand 05.10.2017"<<endl;
	
	
	
	}
bool readEvent(){
	
	
	
	
	
}

int main(int argc, char*argv[])  {
	
	cout<<"Beginn Main"<<endl;
	

	
	int ch;
		
	vector<event*> allEvents;	//	http://www.cplusplus.com/forum/general/55651/
	
	string outputFile, logFile, inputFile;
//	vector <string> inputFiles;		
	

	string oneLine;
	long double oneValue;

	
	long double MIN_SIGNAL_DURATION=0.001, MAX_SIGNAL_DURATION=0.5; //in Sekunden tatsaechlich viel kleiner

	
	
	bool verbose=false;

	while ((ch = getopt(argc, argv, "vi:o:l:m:M:b:h?"))!=EOF)
		{	
			cout<<"Beginn Optionen"<<endl;
		switch ((char)ch)
			{
				//case 's':  notSorted = false;
				case 'o':  outputFile = optarg;
					//printf("Output wurde gewaehlt. %s.c_str()\n", outputFile.c_str());
					cout<<"-o"<<endl;
					break;
				case 'l': logFile=optarg;
					cout<<"-l"<<endl;
					break;
				case 'i': inputFile=optarg;
				
					cout<<"-i"<< inputFile<<endl;
						break;
						
				case 'm' :{
				
							stringstream str(optarg);
							str>>MIN_SIGNAL_DURATION;}
					break;
					
				case 'M' : {
						stringstream str(optarg);
						str>>MAX_SIGNAL_DURATION;}
				
					break;
					
				case 'h': Usage();
					return -1;
					break;
				case 'v': verbose = true;
					break;
				//case 'b': b = atoi(optarg);
				//	break;
				//case 'c': column1 = atoi(optarg); //momentan nicht individuell fuer jede Datei
				//	break;
					/*case 'C':column2 = atoi(optarg); momentan nicht moeglich
						break;*/
				//case 'm': maxTimestampsAtOnce = atoi(optarg);
				//	break;
			}
		}
		
	cout<<"Ende Optionen"<<endl;
	
	if(MIN_SIGNAL_DURATION>MAX_SIGNAL_DURATION||MIN_SIGNAL_DURATION<0||MAX_SIGNAL_DURATION<0){
		cout<<"Minimale (-m) und Maximale (-M) Signaldauer vertauscht oder negative Werte eingegeben"<<endl;
		
		return -1;
		
		}
	
	
	ofstream goodTimestamps;
	ofstream log;
	
	goodTimestamps.open(outputFile.c_str());
	log.open(logFile.c_str());
	
	
	
	/*int index=0;
	for (int i = optind; i<argc; i++)
		{
			cout<<"Lese Dateinamen"<<endl;
			inputFiles.push_back("");
			string temp = argv[i];
			inputFiles[index] = temp;
			if (verbose)
			{
				cout<<index<<". Dateiname ist "<<inputFiles[index].c_str()<<"."<<endl;
			}
			index++;
		}*/
	ifstream inputstream(inputFile.c_str());

	int maxValues=INT_MAX;
	int counter=0;
	size_t firstSpace, secondSpace, thirdSpace;

	long double oneTimestamp,oneAgo;
	
	char oneType;
	int oneAccuracy, oneSatellite;
	long int oneCounter;
	
	string accuracystring, typestring, timestring, counterstring, satellitestring, agostring;	//substring liefert zuerst Strings
	
	
	for (unsigned int i = 0; i<maxValues; i++)		//hoechstens maxValues werden gelesen //Auslagern in eigene Funktion
	{	
		if (inputstream.eof())//Wenn fertig
		{
			break;
		}
		
		getline(inputstream, oneLine);
		stringstream str(oneLine);
		str>>oneValue;
		
		
		//Finde die Leerzeichen,  Bug: findet auch den "."?
		firstSpace=oneLine.find(" ",0);
		if(verbose) cout<<"In "<<oneValue<<" 1. Leerzeichen: "<<firstSpace<<endl;
		
		if (firstSpace>500000){		//DEr Inputstream hoert zu spaet auf, das fuehrt zu unsinnig grossen Werten fuer firstSpace .... 
			break;					//50000 ist willkuerlich, muss groesser als alle denkbaren timestamps sein aber nicht zu gross
			
			}
		timestring=oneLine.substr(0,firstSpace);
		
		secondSpace=oneLine.find(" ",firstSpace+1);	//+1 weil sonst das gefundenene " " wiedergefunden wird
		if(verbose) cout<<"2. Leerzeichen: "<<secondSpace<<endl;
		typestring=oneLine.substr(firstSpace,secondSpace);
		
		
		//Datentypanpassung des eventTypes hier noetig um switch zu verwenden
		stringstream str3(typestring);
		str3>>oneType;
		
		switch (oneType){		//In Abhaengigkeit des Typs mussen andere Dinge gelesen werden
			
		
			case 'r':
			{}
			
			//KEIN break; Soll durchfallen, so dass bei r und f der gleiche Code verwendet wird
			
			case 'f':
			
			
			
			{		
				
				
				
				//Diese Klammer ist wichtig https://stackoverflow.com/questions/5685471/error-jump-to-case-label
				
				if(verbose) cout<<"Rising oder Falling gelesen";
				signalEvent* oneSignalEvent=new signalEvent();
				oneSignalEvent->timestamp=0;	//-> statt . weil Pointer http://www.cplusplus.com/forum/general/55651/
				oneSignalEvent->eventType='u';		//Initialisierung geht besser
				oneSignalEvent->accuracy=0;
				oneSignalEvent->eventCounter=0;
				
				thirdSpace=oneLine.find(" ",secondSpace+1);
				accuracystring=oneLine.substr(secondSpace, thirdSpace);
				
				counterstring=oneLine.substr(thirdSpace);
				
				
				//Typanpassung
				stringstream str2(timestring);
				str2>>oneTimestamp;
				
			
				
				stringstream str4(accuracystring);
				str4>>oneAccuracy;
				
				stringstream str5(counterstring);
				str5>>oneCounter;
				
				
				cout <<"Zeile: "<<i <<": "<<firstSpace<<";"<<secondSpace<<";"<<thirdSpace<<endl;
				
				//Ans Event schreiben
				oneSignalEvent->timestamp=oneTimestamp;
				oneSignalEvent->eventType=oneType;
				oneSignalEvent->accuracy=oneAccuracy;
				oneSignalEvent->eventCounter=oneCounter;
				

				
				//Neues Event anhaengen an allEvents
				cout<<"Das wurde gelesen: ";
				cout<<"Timestamp "<<oneSignalEvent->timestamp<<" "<<",Counter "<<oneSignalEvent->eventCounter<< ",Typ "<<oneSignalEvent->eventType<< ",Accuracy "<<oneSignalEvent->accuracy<<endl;
				
				
				
				
				allEvents.push_back(oneSignalEvent);
				
				//cout<<"Hier war pushback"<<endl;
			
			
			break;
		}
			
			case 's':
			{		//KLammer auch wichtig
					
					
					
				satelliteEvent* oneSatelliteEvent=new satelliteEvent();
				oneSatelliteEvent->satellites=0;
				oneSatelliteEvent->ago=0;
				
				
			
				thirdSpace=oneLine.find(" ",secondSpace+1);
				satellitestring=oneLine.substr(secondSpace, thirdSpace);
				
				agostring=oneLine.substr(thirdSpace);
				
				
				//Typanpassung
				stringstream str2(timestring);
				str2>>oneTimestamp;
				
			
				
				stringstream str4(satellitestring);
				str4>>oneSatellite;
				
				stringstream str5(agostring);
				str5>>oneAgo;
				
				
				cout <<"Zeile: "<<i <<": "<<firstSpace<<";"<<secondSpace<<";"<<thirdSpace<<endl;
				
				//Ans Event schreiben
				oneSatelliteEvent->timestamp=oneTimestamp;
				oneSatelliteEvent->eventType=oneType;
				oneSatelliteEvent->satellites=oneSatellite;
				oneSatelliteEvent->ago=oneAgo;
				

				
				//Neues Event anhaengen an allEvents
				cout<<"Das wurde gelesen: ";
				cout<<"Timestamp "<<oneSatelliteEvent->timestamp<<"Typ "<<oneSatelliteEvent->eventType<< ",Satelliten "<<oneSatelliteEvent->satellites<<"Ago "<< oneSatelliteEvent->ago<<endl;
				
				
				
				
				allEvents.push_back(oneSatelliteEvent);
				
				cout<<"Hier war pushback"<<endl;
				
			break; 
		}
		
			case 'u':{} //KEIN break
			default:
			{	event* oneUnknownEvent=new event;
					stringstream str2(timestring);
				str2>>oneTimestamp;
				oneUnknownEvent->timestamp=oneTimestamp;
				oneUnknownEvent->eventType='u';
				
				cout<<"Timestamp "<<oneUnknownEvent->timestamp<<"Typ "<<oneUnknownEvent->eventType<< endl;
				
				allEvents.push_back(oneUnknownEvent);
				
			break;}
	
		
	
	}}
	if(verbose){
		
		cout<<"Diese Daten wurden gespeichert"<<endl;
		for (unsigned int i = 0; i<allEvents.size(); i++)
		{ 	
			
			
			
			switch(allEvents[i]->eventType){
			
			
			case'f':{}
			case'r':{
				
				cout<<"Timestamp "<<dynamic_cast <signalEvent*>(allEvents[i])->timestamp<<" "<<",Counter "<<dynamic_cast <signalEvent*>(allEvents[i])->eventCounter<< ",Typ "<<dynamic_cast <signalEvent*>(allEvents[i])->eventType<< ",Accuracy "<<dynamic_cast <signalEvent*>(allEvents[i])->accuracy<<endl;
				break;
				}
				
			case's':
				cout<<"Timestamp "<<dynamic_cast <satelliteEvent*>(allEvents[i])->timestamp<<"Typ "<<dynamic_cast <satelliteEvent*>(allEvents[i])->eventType<< ",Satelliten "<<dynamic_cast <satelliteEvent*>(allEvents[i])->satellites<<"Ago "<< dynamic_cast <satelliteEvent*>(allEvents[i])->ago<<endl;
		
		break;
		
			case 'u':
			
			cout<<"Timestamp "<<allEvents[i]->timestamp<<" Typ "<<allEvents[i]->eventType<<endl;
		}
		
		
		
		}
	
	}
	long int sumLostEvents=0;
	long int prevGoodSignalEvent;
		cout<<allEvents.size()<<endl;
	if(verbose){
		cout<<"Suche ersten eventCounter"<<endl;
		
		}
		
		prevGoodSignalEvent=0;
	for (unsigned int i=0; i<allEvents.size(); i++){	//Vor dem ersten ist nicht festellbar, ob etwas verloren wurde /7Problem: Das erste muss kein signalEvent sein
			if (allEvents[i]->eventType=='r'){	//Erzeugt Speicherzugriffsfehler?
				
				prevGoodSignalEvent=(dynamic_cast <signalEvent*>(allEvents[i])->eventCounter)-1;	
				break;
				
				}
		
		}
		
				cout<<"Event wird geprueftx"<<endl;
	
	long double risingTime, fallingTime, signalDuration;
	
	
	bool enoughSatellites=true;
	

	
	for (unsigned int i = 0; i<allEvents.size(); i++){	
		
			//Alle Events werden durchgegangen, wenn f auf r direkt folgt, und genug Satelliten da sind, ist es ein gutes Event
			
		if (verbose){
			cout<<"Event wird geprueft"<<endl;
			
			}
		if(allEvents[i]->eventType=='r'
		&&allEvents[i+1]->eventType=='f'
		&&enoughSatellites
		&&dynamic_cast <signalEvent*>(allEvents[i])->eventCounter==dynamic_cast <signalEvent*>(allEvents[i+1])->eventCounter)	//Pruefe, ob auf rising ein falling folgt //Bei den Typ Pruefungen steht kein dynamic_cast, weil man ja noich nicht weiss was es ist und eventType member der Klasse event ist
		{	
			risingTime=allEvents[i]->timestamp;
			fallingTime=allEvents[i+1]->timestamp;
			
			cout<< "Nach Rising: "<<dynamic_cast <signalEvent*>(allEvents[i])->eventCounter<<" kommt falling. Nach Zeit: "<< fallingTime-risingTime<< endl;
			cout<<(dynamic_cast <signalEvent*>(allEvents[i])->eventCounter)-prevGoodSignalEvent<<endl;
			
			if(fallingTime-risingTime<MIN_SIGNAL_DURATION||fallingTime-risingTime<MAX_SIGNAL_DURATION){		//sollte evtl oben eingebaut werden oder oben sollte geteilt werden
				
				cout<<"Zeitgrenzen ueberschritten"<<endl;
				log<<"Zeit"<<endl;
				} else { //Das event ist gut genug, um aufgeschrieben zu werden, // Fuer Weiterverabeitung mit compare werden nur die rising Timestamps benoetigt
					
		
						
						goodTimestamps<<risingTime<<endl;
						
					
					}
			
			
			if ((dynamic_cast <signalEvent*>(allEvents[i])->eventCounter)-prevGoodSignalEvent!=1)		//Pruefe Abstand (Counter) zu vorheriger Kombination aus rising und Falling
				{
				sumLostEvents+=dynamic_cast <signalEvent*>(allEvents[i])->eventCounter-prevGoodSignalEvent-1;
				cout<<"Jetzt wurden "<<dynamic_cast <signalEvent*>(allEvents[i])->eventCounter-prevGoodSignalEvent-1<<" Events verloren. ("
				<<prevGoodSignalEvent+1<<" bis "<< dynamic_cast <signalEvent*>(allEvents[i])->eventCounter
				<< ") Insgesamt schon: " <<sumLostEvents <<endl;
				
				
				
				}
			
			prevGoodSignalEvent=dynamic_cast <signalEvent*>(allEvents[i])->eventCounter;
			
			
		
			} else if(allEvents[i]->eventType=='r'&&enoughSatellites)		//Fehlendes Falling
			{		//nicht schoen aber geht
				cout<<"Nach Rising: "<<dynamic_cast <signalEvent*>(allEvents[i])->eventCounter<< " kommt kein Falling"<<endl;
				
				
			} else if(allEvents[i]->eventType=='s'){		//SatellitenEvent
				if(dynamic_cast <satelliteEvent*>(allEvents[i])->satellites<3){
					enoughSatellites=false;
					
					cout<<"Nicht genug Satelliten"<<endl;
					
					
					}
					
				if(dynamic_cast <satelliteEvent*>(allEvents[i])->satellites>3){
						enoughSatellites=true;
						
					cout<<"Genug Satelliten"<<endl;}
					
					}
				
			}	
			
	log<<"verlorene Events: "<<sumLostEvents<<endl;
	goodTimestamps.close();
	log.close();
	
	cout<<"Fertig"<<endl;

}
