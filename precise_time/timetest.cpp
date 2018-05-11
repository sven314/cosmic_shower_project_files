
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




precise_time stringtime1("1502047692.000000030");
precise_time stringtime2("1502047692.000000030");
cout<< stringtime1.sec()<<":"<<stringtime1.nsec();


//cout<<time1;
//cout<<(time1.sec()+time2.sec());

cout<<endl;


if(stringtime1!=stringtime2){cout<<"TRUE";}else{ cout<<"FALSE";}
cout<<"END"<<endl;





//c=a-b;
//cout<<c<<endl;
//time3=time1+time2);

//cout<<time3;



}
