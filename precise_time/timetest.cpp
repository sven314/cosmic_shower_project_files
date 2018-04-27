
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




precise_time stringtime("2.1234567890");
precise_time stringtime2("-2.1");
cout<< stringtime.sec()<<":"<<stringtime.nsec();
precise_time product(0.0);

//cout<<time1;
//cout<<(time1.sec()+time2.sec());

cout<<endl;
double a=0, b=0, c;

product=5LL*stringtime;
cout<<stringtime<<endl<<product<<endl;






//c=a-b;
//cout<<c<<endl;
//time3=time1+time2);

//cout<<time3;



}
