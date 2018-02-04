#include<stdio.h>
#include"lab3.h"
struct time TimeDifference(struct time t1,struct time t2);
int main()
{
	//for time1: 3:45:15 and time2: 9:44:03
	t1.hours=3;
	t1.minutes=45;
	t1.seconds=15;
	t2.hours=9;
	t2.minutes=44;
	t2.seconds=03;
	struct time t3=TimeDifference(t1,t2);
	printf("The diff time is %d:%d:%d\n",t3.hours,t3.minutes,t3.seconds);
	
	//for time1: 1:30:45 and time2: 16:30:45 
	t4.hours=1;
	t4.minutes=30;
	t4.seconds=45;
	t5.hours=16;
	t5.minutes=30;
	t5.seconds=45;
	struct time t6=TimeDifference(t4,t5);
	printf("The time after difference is %d:%d:%d\n",t6.hours,t6.minutes,t6.seconds);
	return 0;
}
int numSeconds(int hours,int minutes,int seconds)
{
	TRACE;
	int total_seconds;
	total_seconds=(hours*MINUTES_IN_AN_HOUR*SECONDS_IN_A_MINUTE)+(minutes*SECONDS_IN_A_MINUTE)+seconds;
	return(total_seconds);
}

struct time TimeDifference(struct time t1,struct time t2)
{
	//to calculate the total time seconds in each struct
	int t1Seconds,t2Seconds,diffSec; // local variables

	t1Seconds=numSeconds(t1.hours,t1.minutes,t1.seconds);
	t2Seconds=numSeconds(t2.hours,t2.minutes,t2.seconds);
	diffSec=(t2Seconds-t1Seconds);
	printf("the time diff is %d\n",diffSec);
	struct time t3;
	t3.hours=(diffSec/(MINUTES_IN_AN_HOUR*SECONDS_IN_A_MINUTE));
	diffSec=diffSec-(t3.hours*MINUTES_IN_AN_HOUR*SECONDS_IN_A_MINUTE);
	t3.minutes=diffSec/SECONDS_IN_A_MINUTE;
	diffSec=diffSec-(t3.minutes*SECONDS_IN_A_MINUTE);
	t3.seconds=diffSec;
	return t3;
}
