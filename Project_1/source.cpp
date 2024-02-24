#include <stdio.h>
#incldue <stdlib.h>
//Function Declaration
//Here 'a' is raised to the power 'p'
long int calcPower(int a, int p);

int main()
{
	//Setting value of variable a = 13;
	int a = 13;
	//Setting value of varible p = 5;
	int p = 5;
	// declaing a variable b of long integer type
	long int b;
	//Calling function myPower() and storng it's output in 'b' variable
	b = calcPower(a, p);

	printf("%d raised to the power %d is %ld \n", a, p, b);

	//Using Some extra Test Cases with Power variable
//Case 1 for Negative Power
	printf("Case 1 : \n");
	p = -1;
	//Calling function myPower() and storng it's output in 'b' variable
	b = calcPower(a, p);
	//Printing the output at Console
	printf("%d raised to the power %d is %ld \n", a, p, b);

	//Case 2 for Zero Power
	printf("Case 2 : \n");
	p = 0;

	//Calling function myPower() and storng it's output in 'b' variable
	b = calcPower(a, p);
	//Printing the output at Console
	printf("%d raised to the power %d is %ld \n", a, p, b);

	return 0;
}




long int calcPower(int a, int p)
{
	//Consider 3 cases in calculating power
	//Case 1 : Power number should be a positive. Although it could be negative but not considering that
	if (p < 0)
	{
		printf("Power can not be nagative number . Kindly use Positive number\n");
		exit(1);
	}
	//Case 2 : For rest of the numbers we will be using for loops to calculate power
	//consider an example 5^3 = 5x5x5 i.e 5 is being multiplied 3 times
	//Declaring a variable to store answer
	long int ans = 1;
	for (int i = p; i > 0; i--)
	{
		//Multiplying 'a' , 'p' times!
		ans = ans * a;
	}
	return ans;
}