

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

#include <algorithm>

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
			
			
			
			nsecnumberstr=str.substr(dec_div+1,min(int(9), int(str.length()-1-dec_div)));
			
	
			long long n=9-nsecnumberstr.length();
			
		
			long long int exp=pow(10LL,n);
			
		
		
				
	
			stringstream str2(nsecnumberstr);
			str2>>newnsec;
		
			nsecnumber=newnsec*exp;
		
			
			
        }
        
        
        //Copy-Konstruktor
  precise_time::precise_time(const precise_time& time){
	  secnumber=time.secnumber;
	  nsecnumber=time.nsecnumber;
	  signdigit=time.signdigit;
	  
	  
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
	//cout<<"bin "<<time1.nsec()<<"+"<<time2.nsec()<<endl;
	
	int resultsign;
	long long seconds, nanoseconds;
	precise_time bigger(0.0);
	precise_time smaller(0.0);
	if(time1.signdigit==time2.signdigit)  {
		
		nanoseconds=time1.nsecnumber+time2.nsecnumber;
			seconds=time1.secnumber+time2.secnumber;
			
		cout<<"precise_time: "<<nanoseconds<<endl;
			if(nanoseconds>=1000000000LL){
				nanoseconds=nanoseconds-1000000000LL;
				seconds++;
				
				}
		if(time1.signdigit==1){
			resultsign=1;
				
				
				
		} else {
			resultsign=-1;}}
			
		else  { 
			
			
			
			if(abs(time1)>abs(time2)){
			
			 
			bigger=time1;
			smaller=time2;
			
			
			}else {
			bigger=time2;
			smaller=time1;
		}
			//cout<<bigger<<endl<<smaller<<endl;
			resultsign=bigger.signdigit;
			
			nanoseconds=bigger.nsecnumber-smaller.nsecnumber;
			//cout<<nanoseconds<<endl;;
			seconds=bigger.secnumber-smaller.secnumber;
	
			if(nanoseconds<0){
				
				nanoseconds=1000000000LL+nanoseconds;
				seconds--;
				
				}
			
				
				}
			

	

	
    precise_time result(seconds, nanoseconds, resultsign);
    
  
    return result;
}

 precise_time operator *( double factor,  precise_time time) {
	 
	 long double newsec, newnsec;
	 
	 int newsign, factorSign;
	 if(factor<0){factorSign=-1;}
	 else{factorSign=1;}
	 
	 factor=factorSign*factor;
	 
	 newsign=factorSign*time.sign();
		 
		long double a=factor*time.sec();
		long double b=factor*time.nsec();
		
		cout<<b<<" "<<int(b)<<endl;
		int u=int(b)/1000000000LL;
		cout<<"Ueberlauf: "<<u<<endl;
		long double A=int(a)+u;
		
		long double B=round(b)-1000000000LL*u+(a-int(A))/(1000000000LL);
		
	
		 
	precise_time result((long long)(A), (long long)(B), newsign);
	
	 
		 
	return result;
	 
	 
	 
	 
	 }
	 
 precise_time operator *( precise_time time,  double factor) {
	 

	precise_time result(0.0);
	
	result=factor*time;
		 
	return result;
	 
	 
	 
	 
	 }
 
precise_time operator *(long long factor, precise_time time){
	
	
	
	
	
	
	
	 
	 
	 
	 int newsign, factorSign;
	 if(factor<0){factorSign=-1;}
	 else{factorSign=1;}
	 
	 factor=factorSign*factor;
	 
	 newsign=factorSign*time.sign();
		 
		long long int a=factor*time.sec();
		long long int b=factor*time.nsec();
		
		cout<<b<<" "<<b<<endl;
		int u=b/1000000000LL;
		cout<<"Ueberlauf: "<<u<<endl;
		long long A=int(a)+u;
		
		long long B=round(b)-1000000000LL*u+(a-A)/(1000000000LL);
		
	
		 
	precise_time result(A, B, newsign);
	
	 
		 
	return result;
	 
	 
	 
	 
	 }
	 
precise_time operator *( precise_time time,  long long factor) {
	
	
	
	precise_time result(0.0);
	
	result=factor*time;
		 
	return result;
	
}
	 
precise_time operator /( precise_time time,  double divisor) {
	 

	precise_time result(0.0);
	
	result=(1/divisor)*time;
		 
	return result;
	 
	 
	 
	 
	 }
 
 precise_time operator /( precise_time time,  long long divisor) {
	 
	 
	 
	 return time/(double(divisor));
	 
	 
	 
	 }
 
 precise_time operator -( precise_time time1,  precise_time time2)
{	
	//cout<<"bin. -"<<endl;
    precise_time result=(time1+(-(time2)));
    
    
    
  
    return result;
}




 precise_time operator -( precise_time time) {	
	precise_time tmp(time.sec(),time.nsec(),-time.signdigit);
		//cout<<"un. -"<<endl;


	return tmp;
	}
	

std::ostream& operator <<(std::ostream& stream, precise_time time)  {

		
	if(time.sign()<0){
		

		stream<<"-";
	
		}
	stream<<setw(10)<<setfill(' ');
	
	stream<<time.sec()<<".";
	
	stream<<setw(9)<<setfill('0');
	stream<<time.nsec();
	return stream;
	}
	
	
	

