
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




precise_time stringtime("1502047692.000000030");
precise_time stringtime2("1502047691.000000037");
cout<< stringtime.sec()<<":"<<stringtime.nsec();
precise_time product(0.0);

//cout<<time1;
//cout<<(time1.sec()+time2.sec());

cout<<endl;
double a=0, b=0, c;

product=stringtime-stringtime2;
cout<< product.sec()<<":"<<product.nsec();
cout<<product<<endl;






//c=a-b;
//cout<<c<<endl;
//time3=time1+time2);

//cout<<time3;



}
