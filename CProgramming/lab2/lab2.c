#include<stdio.h>
#include<limits.h>
#include<stdlib.h>

#define LENGTH 20
#define MAX_BIT 31

int main()
{
	// Maximum length of the series
	int i,j,f_series[LENGTH];
	//Assigning the first 2 values of the series to the array
	f_series[0]=0;
	f_series[1]=1;
	// Loop for adding the previous 2 values in the current position
	for(i=2;i<=LENGTH;i++)
	{
		f_series[i]=f_series[i-1]+f_series[i-2];
	}
	printf("\n The fibonacci sequence is : \n");
	// To print the values in the series
	for(i=0;i<LENGTH;i++)
		printf("%d ",f_series[i]);
	printf("\n");
	//Binary printer
	int a=2;
	int b=255;
	int c=32;
	int d=-1;
	int e=-255;
	int mask=1;
	//Print the binary representation of 2
	printf("\n The binary representation of 2 is    : ");

	for(j=MAX_BIT;j>=0;j--)
	{
		printf("%d",(a>>j)&mask);
	}
	//Print the binary representation of 255
	printf("\n The binary representation of 255 is  : ");

	for(j=MAX_BIT;j>=0;j--)
	{
			printf("%d",(b>>j)&mask);
	}
	//Print the binary representation of 32
	printf("\n The binary representation of 32 is   : ");

	for(j=MAX_BIT;j>=0;j--)
	{
			printf("%d",(c>>j)&mask);
	}
	//Print the binary representation of -1
	printf("\n The binary representation of -1 is   : ");

	for(j=MAX_BIT;j>=0;j--)
	{
			printf("%d",(d>>j)&mask);
	}
	//Print the binary representation of -255
	printf("\n The binary representation of -255 is : ");

	for(j=MAX_BIT;j>=0;j--)
	{
			printf("%d",(e>>j)&mask);
	}
	printf("\n");

	//Printing random binary value
	int random_number;
	random_number=(rand() % INT_MAX) + (rand() % INT_MIN);
	printf("\n The random number generated is : %d\n",random_number);
	printf("\n The binary representation is : ");
	for(j=MAX_BIT;j>=0;j--)
		printf("%d",(random_number>>j)&mask);
	printf("\n");

	return 0;
}
