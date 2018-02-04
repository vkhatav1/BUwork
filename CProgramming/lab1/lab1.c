#include<stdio.h>
#include<math.h>//this header file is required for pow() function
int main()
{
	float x=2.55,exp1_result,exp2_result;
	printf("Hello World!\n");//Result for part 1(A) of the assignment

	//Expression 1
	exp1_result=(3*pow(x,3.0))-(5*pow(x,2.0))+6;
	printf("The result of expression1 for x=2.55 is %e\n",exp1_result);//%e is used to print the result in scientific notation
	//Result for Part 2(A) of the assignment

	//Expression 2
	exp2_result=(3.31*pow(10.0,-8)*2.01*pow(10.0,-7))/((7.16*pow(10.0,-6))+2.01*pow(10.0,-8));
	printf("The result of expression2 is %e\n",exp2_result);//Result for Part 2(A) of the assignment

	//Next largest even multiple

	int i,j,next_multiple1,next_multiple2,next_multiple3;
	i=365; j=7;//Pre-assigned values
	printf("\n\t    i \t  j \tNext Multiple\n");
	next_multiple1=i+j-i%j;
	printf("\t %5d \t %2d \t%5d\n",i,j,next_multiple1);//the values between %d is used to get formatted output

	i=12258; j=28;//Pre-assigned values
	next_multiple2=i+j-i%j;
	printf("\t %5d \t %2d \t%5d\n",i,j,next_multiple2);

	i=996; j=4;//Pre-assigned values
	next_multiple3=i+j-i%j;
	printf("\t %5d \t %2d \t%5d\n",i,j,next_multiple3);

	//Type casting

	printf("Type casting to integer\n");

	printf("\nVARIABLE NAME\tORIGINAL VALUE\tVALUE AFTER TYPE CASTING\n");
	//the results of the expressions are typecasted to integer
	printf("exp1_result\t%e\t%d\n",exp1_result,(int)exp1_result);
	printf("exp2_result\t%e\t%d\n",exp2_result,(int)exp2_result);
	printf("next_multiple1\t%d\t\t%d\n",next_multiple1,(int)next_multiple1);
	printf("next_multiple2\t%d\t\t%d\n",next_multiple2,(int)next_multiple2);
	printf("next_multiple3\t%d\t\t%d\n",next_multiple3,(int)next_multiple3);

	printf("\nType casting to double\n");
	//the results of the expressions are typecasted to double
	printf("\nVARIABLE NAME\tORIGINAL VALUE\tVALUE AFTER TYPE CASTING\n");
	printf("exp1_result\t%e\t%f\n",exp1_result,(double)exp1_result);
	printf("exp2_result\t%e\t%f\n",exp2_result,(double)exp2_result);
	printf("next_multiple1\t%d\t\t%f\n",next_multiple1,(double)next_multiple1);
	printf("next_multiple2\t%d\t\t%f\n",next_multiple2,(double)next_multiple2);
	printf("next_mutliple3\t%d\t\t%f\n",next_multiple3,(double)next_multiple3);

	printf("\nType casting to char\n");
	//the results of the expressions are typecasted to char
	printf("\nVARIABLE NAME\tORIGINAL VALUE\tVALUE AFTER TYPE CASTING\n");
	printf("exp1_result\t%e\t%c\n",exp1_result,(char)exp1_result);
	printf("exp2_result\t%e\t%c\n",exp2_result,(char)exp2_result);
	printf("next_multiple1\t%d\t\t%c\n",next_multiple1,(char)next_multiple1);
	printf("next_multiple2\t%d\t\t%c\n",next_multiple2,(char)next_multiple2);
	printf("next_multiple3\t%d\t\t%c\n",next_multiple3,(char)next_multiple3);

	return 0;
}


