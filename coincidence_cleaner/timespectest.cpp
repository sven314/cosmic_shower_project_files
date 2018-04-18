#include <ctime>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

struct precise_time{
	
	long int sec, nsec;
	   precise_time(long int secn=0, long int nsecn=0) : sec(secn), nsec(nsecn)
        {
        }
friend bool operator <(const precise_time& time1, const precise_time& time2)		//ohne friend wird this als linkes Argument genommen
{
    if (time1.sec == time2.sec)
        return time1.nsec < time2.nsec;
    else
        return time1.sec < time2.sec;
}


friend bool operator >(const precise_time& time1, const precise_time& time2)
{
    if (time1.sec == time2.sec)
        return time1.nsec > time2.nsec;
    else
        return time1.sec > time2.sec;
}
friend const precise_time operator +(const precise_time& time1, const precise_time& time2)
{
    precise_time *result= new precise_time();
    
    result->sec=time1.sec+time2.sec+((time1.nsec+time2.nsec)%1000000000);
    result->nsec=(time1.sec+time2.sec)-1000000000*((time1.sec+time2.sec)%1000000000);
    return result;
}

};
int main() {


precise_time *time1=new precise_time(1,1);
precise_time *time2=new precise_time(1,1);


time1->sec=1;
time2->sec=2;


//cout<<time1;
cout<<(time1->sec+time2->sec);

//time3=time1+time2);

//cout<<time3;



}
