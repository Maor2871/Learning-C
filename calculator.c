#include <stdio.h>

void calculator()
{
	int num1, num2, is_valid;
	float result;
	char operator;
	
	// --- Input ---
	
	// Opening message.
	printf("\n--- Welcome to Mini-Calculator ---\n");
	printf("Mini calculator can calculate summation, subtraction, multiplication and division of two integers.\n\n");
	
	// Get the first number.
	printf("Please enter the first number: ");
	is_valid = scanf("%d", &num1);
	if (!is_valid) { printf("bad input. aborting calculator."); return; }
	
	// Get the operator.
	printf("Please enter the operator (+, -, *, /): ");
	scanf(" %c", &operator);
	if (operator != '+' && operator != '-' && operator != '*' && operator != '/') { printf("Bad input. aborting calculator."); return;}
	
	// Get the second number.
	printf("Please enter the second number: ");
	is_valid = scanf("%d", &num2);
	if (!is_valid) { printf("bad input. aborting calculator."); return; }
	
	// --- Analysis ---
	
	// Switch case the operator and calculate the result.
	switch (operator)
	{
		case '+':
			result = num1 + num2;
			break;
		
		case '-':
			result = num1 - num2;
			break;
		
		case '*':
			result = num1 * num2;
			break;
		
		case '/':
			
			// Make sure to not divide by 0.
			if (num2 == 0) { printf("What does it mean to divide by 0? aborting calculator."); return;}
			
			result = (float)num1 / num2;
			break;
			
		default:
		
			// Not supposed to get here. Just in case.
			printf("\nSomething went wrong. aborting calculator.");
			return;
	}
	
	// Print the calculation with the calculated result.
	printf("\nCalculation: %d %c %d = %f", num1, operator, num2, result);	
}

int main()
{
	calculator();
}