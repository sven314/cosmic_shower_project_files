//gcc -O5 -o timestamp_diffmean_systime timestamp_diffmean_systime.c -lrt -lpigpio

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <pigpio.h>
#include <string.h>

typedef enum {SIGNAL, PPS, DCF} SIGNALMODE;
const SIGNALMODE mode = PPS;

const long int T0 = 1458380000L;

//static volatile uint32_t oldTS = 0;

//struct timespec tp,oldTp,buffTp;
uint32_t oldTick = 0;
long long int t, llTick=0;
long double linOffset,linSlope;
long double ppsAvg = 0.;
int triggerLevel = 1;

struct timespec tLast;

int N = 100;
    int nrSamples=0;
    int arrayIndex=0;
    long long int diff_array[32768];
    long long int tick_array[32768];
int avgBufSize = 8;

void calcMean(long long int *array, int nrSamples, long long int *meanInt, double* meanFrac)
{
   if (nrSamples<1) return;
   double akku = 0.;

   int i;
   long long int offs=array[0];

   for (i=0; i<nrSamples; i++) {
      long long int val=array[i]-offs;
      akku+=val;
   }
   akku/=nrSamples;
   while (akku<0.) {
      akku=akku+1.;
      offs--;
   }
   int remainder=(int)akku;
   *meanInt=offs+remainder;
   *meanFrac=akku-(double)remainder;
}

inline static long double sqr(long double x) {
        return x*x;
}

/*
 linear regression algorithm
 taken from:
   http://stackoverflow.com/questions/5083465/fast-efficient-least-squares-fit-algorithm-in-c
 parameters:
  n = number of data points
  xarray,yarray  = arrays of data
  *offs = output intercept
  *slope  = output slope
 */
void calcLinearCoefficients(int n, long long int *xarray, long long int *yarray,
			    long double *offs, long double* slope)
{
   if (n<3) return;

   long double   sumx = 0.0;                        /* sum of x                      */
   long double   sumx2 = 0.0;                       /* sum of x**2                   */
   long double   sumxy = 0.0;                       /* sum of x * y                  */
   long double   sumy = 0.0;                        /* sum of y                      */
   long double   sumy2 = 0.0;                       /* sum of y**2                   */

   int ix=arrayIndex;
   if (ix==0) ix=n-1;
   else ix--;
   
   long long int offsx=xarray[ix];
   long long int offsy=yarray[ix];
//    long long int offsy=0;

   int i;
   for (i=0; i<n; i++) {
          sumx  += xarray[i]-offsx;       
          sumx2 += sqr(xarray[i]-offsx);  
          sumxy += (xarray[i]-offsx) * (yarray[i]-offsy);
          sumy  += (yarray[i]-offsy);
          sumy2 += sqr(yarray[i]-offsy); 
   }


   double denom = (n * sumx2 - sqr(sumx));
   if (denom == 0) {
       // singular matrix. can't solve the problem.
       *slope = 0;
       *offs = 0;
//       if (r) *r = 0;
       return;
   }

   long double m = (n * sumxy  -  sumx * sumy) / denom;
   long double b = (sumy * sumx2  -  sumx * sumxy) / denom;
   
   *slope=m;
   *offs=b+offsy;
//    *offs=b;
//   printf("offsI=%lld  offsF=%f\n", offsy, b);
   
}


void aFunction(int gpio,int level, uint32_t tick)
{
   if(level==triggerLevel)
   {
      struct timespec tStart, tEnd;
//      clock_gettime(CLOCK_MONOTONIC, &tp);
      clock_gettime(CLOCK_REALTIME, &tStart);
      double deltaT = tStart.tv_sec-tLast.tv_sec;
      deltaT += (tStart.tv_nsec-tLast.tv_nsec)*1e-9;
      //if ((mode==PPS || mode == DCF) && deltaT<0.7) return;
      long long int timestamp = llTick+tick; // TS in us, BCM clock

      calcLinearCoefficients((nrSamples<N)?nrSamples:N, tick_array, diff_array,
			    &linOffset, &linSlope);
      long double meanDiff=linOffset;
      int ix=arrayIndex;
      if (ix==0) ix=nrSamples-1;
      else ix--;
   
      long double dx=timestamp-tick_array[ix];
      long double dy=linSlope*dx;
      meanDiff+=dy;
            
      long long int meanDiffInt=(long long int)meanDiff;
      double meanDiffFrac=(meanDiff-(long long int)meanDiff);
      timestamp+=meanDiffInt; // add diff to real time
      long int ts_sec=timestamp/1000000+T0; // conv. us to s
      long int ts_nsec=1000*(timestamp%1000000);
      ts_nsec+=(long int)(1000.*meanDiffFrac);
      
      long double ppsOffs = (ts_sec-tStart.tv_sec)+ts_nsec*1e-9;
      if (fabs(ppsOffs)>1.) return;
      ppsAvg = ppsAvg*avgBufSize/(avgBufSize+1.)+ppsOffs/(avgBufSize+1.);
      
      // Convert absolute Time to readable Time
//      long double absolut = ts_sec + (ts_nsec/1000000000);
      time_t t = tStart.tv_sec;
      struct tm *tm = localtime(&t);
      char date[80];
      strftime(date, sizeof(date), "%Y.%m.%d %H:%M:%S", tm);
      //clock_gettime(CLOCK_REALTIME, &tEnd);
      // calculate dead time
      /*double deadtime = tEnd.tv_sec-tStart.tv_sec;
      deadtime += (tEnd.tv_nsec-tStart.tv_nsec)*1e-9;
      */
      //if (argc == 0)
      //{
      printf("%d %ld.%06d %lld %lld.%03d %s %ld.%09ld %12.9f %12.9f", gpio, tStart.tv_sec,tStart.tv_nsec/1000, llTick+tick,meanDiffInt,(int)(meanDiffFrac*1000.),
	     date, ts_sec, ts_nsec, ppsOffs, ppsAvg);
      //}
      fflush(stdout);
      clock_gettime(CLOCK_REALTIME, &tEnd);
      double deadtime = tEnd.tv_sec-tStart.tv_sec;
      deadtime += (tEnd.tv_nsec-tStart.tv_nsec)*1e-9;
      printf(" %5.1f\n",deadtime*1e+6);
      tLast=tStart;
     }
}

int main (int argc, char** argv)
{
    int ch;
    int ppspin=18;
    int sigpin=-1;
    char dat1[99], dat2[99];
    while ((ch = getopt(argc, argv, "N:p:s:l:n:")) != EOF)
    {
       if ((char)ch == 'N')
       {
           N = atoi(optarg);
	   printf("Regressions-Puffergroesse N wurde gesetzt als: %d\n", N);
       }
       if ((char)ch == 'p')
       {
           ppspin = atoi(optarg);
	   printf("GPIO-PIN %d wurde als PPS-Pin gewaehlt.\n", ppspin);
       }
       if ((char)ch == 's')
       {
	   sigpin = atoi(optarg);
	   printf("GPIO-PIN %d wurde als Signal-Pin gewahelt.\n", sigpin);
       }
       if ((char)ch == 'l')
       {
           triggerLevel = atoi(optarg);
	   printf("trigger level auf %d gesetzt.\n", triggerLevel);
       }
       if ((char)ch == 'n')
       {
           avgBufSize = atoi(optarg);
	   printf("gleitende MW-Laenge wurde gesetzt als: %d\n", avgBufSize);
       }
    }
    argc -= optind;
    argv += optind;

    printf("Anzahl an Argumenten: %d\n", argc);

    if (argc > 2)
    {
	perror("Falsche Eingabe, zu viele Argumente!\n");
    	return -1;
    }

    printf("PPS-Pin: %d\nSignal-Pin: %d\n", ppspin, sigpin);

    if (gpioCfgClock(1,0,0)<0)
    {
        return 1;
    }
    if (gpioInitialise()<0)
    {
        return 1;
    }
    if (ppspin > 0)
    {
    	gpioSetAlertFunc(ppspin, aFunction);
       	gpioSetMode(ppspin, PI_INPUT);
    }
    if (sigpin > 0)
    {
        gpioSetMode(sigpin, PI_INPUT);
        gpioSetAlertFunc(sigpin, aFunction);
    } 
//    gpioSetMode(sigpin, PI_INPUT);
//    gpioSetAlertFunc(sigpin, aFunction);
//    gpioSetMode(ppspin, PI_INPUT);
  //  gpioSetAlertFunc(ppspin, aFunction);


    if (argc > 0 && argc < 3)
    {
        strcpy(dat1, *argv);
        printf("Dateiname1: %s\n", dat1);
        --argc;
        ++argv;
        strcpy(dat2, *argv);
        printf("Dateiname2: %s\n", dat2);
        --argc;
        ++argv;
    }

    uint32_t tick;
    // int N=100;
//     int nrSamples=0,index=0;
//     long long int diff_array[N];
//     long long int tick_array[N];

    struct timespec tp;

    while (1)
    {
//     	sleep(1);
        tick = gpioTick();
//        clock_gettime(CLOCK_MONOTONIC, &tp);
        clock_gettime(CLOCK_REALTIME, &tp);

	tp.tv_sec-=T0;

        if (tick<oldTick)
        {
           llTick = llTick + UINT32_MAX + 1;
        }
        oldTick=tick;
        long long int nr_usecs = tp.tv_sec*1e+6;
        nr_usecs+=tp.tv_nsec/1000;
//        diff_array[index++]=*timer-llTick-tick;
        diff_array[arrayIndex]=nr_usecs-llTick-tick;
	tick_array[arrayIndex]=llTick+tick;
//	arrayIndex++;
        if (++arrayIndex>=N) {
           arrayIndex=0;
        }
        if (nrSamples<N) nrSamples++;
        //calcMean(diff_array, (nrSamples<N)?nrSamples:N, &meanDiffInt, &meanDiffFrac);
//  	calcLinearCoefficients((nrSamples<N)?nrSamples:N, tick_array, diff_array,
//  			    &linOffset, &linSlope);
	usleep(251000L);
    }
    return 0;
}

