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
using namespace std;

class precise_time{
	
	long int secnumber, nsecnumber;
	
	
	
	public: precise_time(long int secn=0, long int nsecn=0) : secnumber(secn), nsecnumber(nsecn) //einfacher Konstruktor
        {
        }
        
       public: precise_time(string str) //Konstruktor zum Lesen aus einen String ####.#####
        {	long int newsec,newnsec;
			string secnumberstr, nsecnumberstr;
			long int dec_div=str.find(".");
			
			secnumberstr=str.substr(0,dec_div);
			
			stringstream str1(secnumberstr);		//4.5 wird als 4 sec +5 nsec gelsen
			str1>>newsec;
			
			secnumber=newsec;
			
			
			
			nsecnumberstr=str.substr(dec_div+1, str.length());
			stringstream str2(nsecnumberstr);
			str2>>newnsec;
			nsecnumber=newnsec;
			
        }

friend bool operator <(const precise_time& time1, const precise_time& time2)		//ohne friend wird this als linkes Argument genommen
{
    if (time1.secnumber == time2.secnumber)
        return time1.nsecnumber < time2.nsecnumber;
    else
        return time1.secnumber< time2.secnumber;
}


friend bool operator >(const precise_time& time1, const precise_time& time2)
{
    if (time1.secnumber == time2.secnumber)
        return time1.nsecnumber > time2.nsecnumber;
    else
        return time1.secnumber > time2.secnumber;
}

friend const precise_time operator +(const precise_time& time1, const precise_time& time2)
{
    precise_time result(time1.secnumber+time2.secnumber+((time1.nsecnumber+time2.nsecnumber)%1000000000),
    (time1.secnumber+time2.secnumber)-1000000000*((time1.secnumber+time2.secnumber)%1000000000));
    
  
    return result;
}


friend ostream& operator <<(ostream &, const precise_time&);


public: long int sec(){return secnumber;}
public: long int nsec(){return nsecnumber;}

};



ostream& operator <<(ostream& stream, precise_time& time){
	
	
	
	stream<<setw(9)<<setfill(' ');
	stream<<time.sec()<<".";
	
	stream<<setw(9)<<setfill('0');
	stream<<time.nsec();
	
	}
int main() {


precise_time time1(1,1);
precise_time time2(1,1);

precise_time stringtime("4.5");



//cout<<time1;
//cout<<(time1.sec()+time2.sec());
cout<<time1;
cout<<endl;
cout<<stringtime;
//time3=time1+time2);

//cout<<time3;



}
