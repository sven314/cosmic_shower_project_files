
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
	
int main() {




precise_time stringtime("2.0");
precise_time stringtime2("-2.1");

precise_time sum(0.0);

//cout<<time1;
//cout<<(time1.sec()+time2.sec());

cout<<endl;
double a=0, b=0, c;

cout<<stringtime<<endl;
cout<<stringtime2<<endl;
sum=stringtime-stringtime2;
cout<<sum<<endl;

sum=2.0*sum;


cout<<sum<<endl;

sum=sum*2.0;

cout<<sum<<endl;
sum=sum/3.0;

cout<<sum<<endl;

//c=a-b;
//cout<<c<<endl;
//time3=time1+time2);

//cout<<time3;



}
