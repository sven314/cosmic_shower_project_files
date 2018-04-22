

#ifndef PRECISE_TIME_H
#define PRECISE_TIME_H



#include <ctime>
#include <stdio.h>


#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <ostream>

#include <string>
#include <iomanip>
#include <math.h>
 




class precise_time{
	#include <ctime>
#include <stdio.h>


#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <ostream>

#include <string>
#include <iomanip>
#include <math.h>
	//Variabeln
	//
	long long secnumber, nsecnumber;
	public: int signdigit;
	
	
	

	
	//Konstruktoren

	
	public: precise_time(long long secn, long long nsecn, int signn);
        
    public: precise_time(double time);  //
      
        
	public: precise_time(std::string str); //Konstruktor zum Lesen aus einen String ...####.#########
   
   
	//Getter
	public: long long  sec();
	public: long long  nsec();
	public: int sign();
	
	
	
	//friends
	
	
	friend  std::ostream& operator << (std::ostream& stream, precise_time& time);
	
	
	friend   precise_time abs( precise_time time1);
	
	friend    precise_time operator +( precise_time time1,  precise_time time2);
	
	friend    precise_time operator *( double factor,   precise_time time);
	
	friend    precise_time operator *( precise_time time, double factor );
	
	friend    precise_time operator /( precise_time time, double divisor );

	friend    precise_time operator -( precise_time time1,  precise_time time2);		//binaeres -

	friend    precise_time operator -( precise_time time);
	
	friend   bool operator <(precise_time time1, precise_time time2);	
	
	friend   bool operator >(precise_time time1, precise_time time2);
	
	friend   bool operator ==(precise_time time1, precise_time time2);	
	
	friend   bool operator !=(precise_time time1, precise_time time2);	
	
	friend   bool operator >=(precise_time time1, precise_time time2);
	
	friend   bool operator <=(precise_time time1, precise_time time2);	

};


//globale Operatoren

//Vergleich
   bool operator <(precise_time time1, precise_time time2);		


  bool operator >( precise_time time1, precise_time time2);


  bool operator ==( precise_time time1, precise_time time2);
  
  
  bool operator !=( precise_time time1, precise_time time2);
  
  bool operator >=( precise_time time1, precise_time time2);
  
  bool operator <=( precise_time time1, precise_time time2);

//Rechnung
	
	precise_time abs( precise_time time1);
	
	precise_time operator +(precise_time time1,  precise_time time2);

	precise_time operator -( precise_time time1,  precise_time time2);		//binaeres -

	precise_time operator -( precise_time time);		//unaeres -

	precise_time operator *(long long factor,  precise_time time);
	
	precise_time operator *(precise_time time, long long factor);
	
	precise_time operator /(precise_time time, long long divisor);
	#include <ctime>
#include <stdio.h>


#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <ostream>

#include <string>
#include <iomanip>
#include <math.h>
	
   std::ostream& operator << (std::ostream& stream, precise_time& time);		//unaeres -

	

#endif
