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
using namespace std;

class precise_time{
	
	long long secnumber, nsecnumber;
	int sign;
	
	
	public: precise_time(long long secn=0, long long nsecn=0) : secnumber(secn), nsecnumber(nsecn) //einfacher Konstruktor
        {
        }
        
    public: precise_time(string str) //Konstruktor zum Lesen aus einen String ...####.#########
        {	long long int newsec,newnsec;
			string secnumberstr, nsecnumberstr;
			long unsigned int dec_div=str.find(".");
			
			secnumberstr=str.substr(0,dec_div);
			
			
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
    precise_time result(time1.secnumber+time2.secnumber+((time1.nsecnumber+time2.nsecnumber)/1000000000LL),
    (time1.secnumber+time2.secnumber)-(time1.secnumber+time2.secnumber)-1000000000LL*((time1.secnumber+time2.secnumber)/1000000000LL));
    
  
    return result;
}

friend const precise_time operator -(const precise_time& time1, const precise_time& time2)
{	long long newsec, newnsec;
	
	
	newsec=time1.secnumber-time2.secnumber;
	newnsec=time1.nsecnumber-time2.nsecnumber;
	
	
	if(newnsec<0){
		newsec=newsec-1;
		newnsec=newnsec+1000000000LL;
		
		
		
		}
	
    precise_time result(newsec,newnsec);
    
    
    
  
    return result;
}



friend ostream& operator <<(ostream &, const precise_time&);


public: long long sec(){return secnumber;}
public: long long nsec(){return nsecnumber;}

};



ostream& operator <<(ostream& stream, precise_time& time){
	
	
	
	stream<<setw(10)<<setfill(' ');
	
	stream<<time.sec()<<".";
	
	stream<<setw(9)<<setfill('0');
	stream<<time.nsec();
	return stream;
	}
int main() {


precise_time time1(1,1);
precise_time time2(1,1);

precise_time stringtime("1.5");
precise_time stringtime2("1.6");

precise_time sum;

//cout<<time1;
//cout<<(time1.sec()+time2.sec());
cout<<time1;
cout<<endl;
double a=4584615844.512545879, b=4584615844.512545879, c;

cout<<stringtime<<endl;
cout<<stringtime2<<endl;
sum=stringtime-stringtime2;
cout<<sum;
c=a-b;
cout<<c<<endl;
//time3=time1+time2);

//cout<<time3;



}
