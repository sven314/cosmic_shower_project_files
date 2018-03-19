//gcc -o differenz_zeilen differenz_zeilen.c -lrt

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

FILE *fp_read;
//FILE *fp_write;
int main(void)
{
//     char Filename1[100]; printf("Insert File you want to compare: "); scanf("%99s", &Filename1[0]);
//     char Filename2[100]; printf("Insert File you want to write to: "); scanf("%99s", &Filename2[0]);

     fp_read = fopen("raspi2_sig2.txt","r"); //fopen(&Filename1[0], "r");
     //fp_write = fopen(&Filename2[0], "a");
     int i = 0;
     long long int arr[1000000];
     long long int a=0;
     for (i = 0; i<1000; i++)
     {
          arr[i]=0;
     }
     int j = 0;
     while (!feof(fp_read))
     {
           fscanf(fp_read, "%lld", &a);
           arr[j]=a;
	   //printf("%lld \n",a);
           j++;
     }
     //printf("%d", j);
     for(i = 1; i<j;i++)
     {
          a = (arr[i]-arr[i-1]);//-1000000;
	  //a = a/1000;
          printf("%lld \n", a);
     }
     fclose(fp_read);
     //fclose(fp_write);
     return 0;
}
