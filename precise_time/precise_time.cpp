#include <ctime>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <ostream>
#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>

#include "precise_time.h"
using namespace std;

	//Variablen
	//const long long NANO=1000000000LL;
	long long secnumber, nsecnumber;
	int signdigit;


	
	
	
//Konstruktoren
	
	
precise_time::precise_time(long long secn=0, long long nsecn=0, int signn=1) : secnumber(secn), nsecnumber(nsecn), signdigit(signn) //einfacher Konstruktor
        {
        }
        
precise_time::precise_time(double time)  //
        {
			if(time<0){
				signdigit=-1;
				time=-time;
				}
			secnumber=static_cast<long long>(time)/1;
			nsecnumber=static_cast<long>((time-secnumber)+1000000000LL);
        }
        
precise_time::precise_time(string str) //Konstruktor zum Lesen aus einen String ...####.#########
        {	long long newsec,newnsec;
			string secnumberstr, nsecnumberstr;
			long unsigned int dec_div=str.find(".");
			
			if((str.substr(0,1)).compare("-")==0){
				
				
				signdigit=-1;
				secnumberstr=str.substr(1,dec_div);
			
				}else  {signdigit=1;
					secnumberstr=str.substr(0,dec_div);
	
					}
			
			
			
			
			stringstream str1(secnumberstr);		
			str1>>newsec;
			
			secnumber=newsec;
			
			
			
			nsecnumberstr=str.substr(dec_div+1, str.length());
			
	
			long long n=9-nsecnumberstr.length();
			
		
			long long int exp=pow(10LL,n);
			
		
		
				
	
			stringstream str2(nsecnumberstr);
			str2>>newnsec;
		
			nsecnumber=newnsec*exp;
		
			
			
        }
        
        
        
        //Getter
        
        

long long precise_time::sec()  {return secnumber;}
long long precise_time::nsec()  {return nsecnumber;}
int precise_time::sign()  {return signdigit;}




		//friends

bool operator <(precise_time time1, precise_time time2)	 	//ohne friend wird this als linkes Argument genommen
{
    if (time1.sec()*time1.signdigit == time2.sec()*time2.signdigit)
        return time1.nsec()*time1.signdigit < time2.nsec()*time2.signdigit;
    else
        return time1.sec()*time1.signdigit< time2.sec()*time2.signdigit;
}

bool operator >( precise_time time1,  precise_time time2) 
{
    if (time1.sec()*time1.sign() == time2.sec()*time2.sign())
        return time1.nsec()*time1.sign() > time2.nsec()*time2.sign();
    else
        return time1.sec()*time1.sign() > time2.sec()*time2.sign();
}

bool operator ==( precise_time time1,  precise_time time2) 
{

        return ((time1.sec()*time1.sign() == time2.sec()*time2.sign())&&(time1.nsec()*time1.sign() == time2.nsec()*time2.sign()));

}

bool operator !=( precise_time time1,  precise_time time2){
	
	return (!(time1==time2));
	
	
	}

bool operator >=( precise_time time1,  precise_time time2) 
{

        return (time1==time2||time1>time2);

}
bool operator <=( precise_time time1,  precise_time time2) 
{

        return (time1==time2||time1<time2);

}

 precise_time abs( precise_time time1) {
	

		
		precise_time time2(time1.sec(), time1.nsec(), 1);
		
		return time2;

	
	
	}
	
 precise_time operator +( precise_time time1,  precise_time time2) 


{
	cout<<"bin "<<time1.nsec()<<"+"<<time2.nsec()<<endl;
	
	int resultsign;
	long long seconds, nanoseconds;
	
	
	if(time1.signdigit==time2.signdigit){
		resultsign=time1.signdigit;
		
		seconds=time1.sec()+time2.sec();
		nanoseconds=time1.nsec()+time2.nsec();
		
		
		}else if(time1.signdigit<time2.signdigit){		//time 1 negativ, time 2 positiv
			
			if(abs(time1)>abs(time2)){
				
				
				
				seconds=time1.sec()-time2.sec();
				nanoseconds=time1.nsec()-time2.nsec();
				resultsign=-1;
				
				
				}else{
					
					seconds=time2.sec()-time1.sec();
					nanoseconds=time2.nsec()-time1.nsec();
					resultsign=1;
					
					
					}
			
			
			}else{		//time 2 negativ, time1 positiv
				
				if(abs(time1)>abs(time2)){
				
				
				
				seconds=time1.sec()-time2.sec();
				nanoseconds=time1.nsec()-time2.nsec();
				resultsign=1;
				
				
				}else{
					
					seconds=time2.sec()-time1.sec();
					nanoseconds=time2.nsec()-time1.nsec();
					resultsign=-1;
					
					
					}
				
				
				
				
				}
	

	
    precise_time result(seconds, nanoseconds, resultsign);
    
  
    return result;
}
 precise_time operator *( double factor,  precise_time time) {
	 
	 double newsec, newnsec;
	 
	 int newsign, factorSign;
	 if(factor<0){factorSign=-1;}
	 else{factorSign=1;}
	 
	 
	 
	 newsign=factorSign*time.sign();
		 
		newnsec=time.nsec()*factor*factorSign;
		
		newsec=factor*time.sec()+(long long)newnsec/1000000000LL;
		
		newnsec=(newsec-(long long) newsec)*1000000000LL+time.nsec()*factor*factorSign-(factor*time.sec()+(long long)newnsec/1000000000LL)*1000000000LL;
		 
	precise_time result((long long)(newsec), (long long)(newnsec), newsign);
	
	 
		 
	return result;
	 
	 
	 
	 
	 }
	 
 precise_time operator *( precise_time time,  double factor) {
	 

	precise_time result(0.0);
	
	result=factor*time;
		 
	return result;
	 
	 
	 
	 
	 }
	 
precise_time operator /( precise_time time,  double divisor) {
	 

	precise_time result(0.0);
	
	result=(1/divisor)*time;
		 
	return result;
	 
	 
	 
	 
	 }
 
 
 
 precise_time operator -( precise_time time1,  precise_time time2)
{	
	cout<<"bin. -"<<endl;
    precise_time result=(time1+(-(time2)));
    
    
    
  
    return result;
}




 precise_time operator -( precise_time time) {	
	precise_time tmp(time.sec(),time.nsec(),-time.signdigit);
		cout<<"un. -"<<endl;


	return tmp;
	}
	

std::ostream& operator <<(std::ostream& stream, precise_time& time)  {

		
	if(time.sign()<0){
		

		stream<<"-";
	
		}
	stream<<setw(10)<<setfill(' ');
	
	stream<<time.sec()<<".";
	
	stream<<setw(9)<<setfill('0');
	stream<<time.nsec();
	return stream;
	}
	
	
	

