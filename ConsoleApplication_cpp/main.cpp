/**
* @file main.cpp
* @author Zhang Zhi
* @brief the implementation of the test of Trimble
* @since 1.0.0
*
* Change Log:
* 
*/


#include <iostream>
#include <vector>
#include <numeric>
#include <cstdlib>
#include "BigInt.h"

const int DIGITAL_BASE = 10;

static int reverseInt(int origInt)
{
	if (origInt < 0)
	{
		std::cout << "Please input an positive integer!" << std::endl;
		return origInt;
	}

	std::vector<int> vec;

	int iDividend = origInt;
	int iQuotient = 0;
	int iRemainder = 0;

	do
	{
		// every iterative each bit of the reversed integer will multiple 10
		for (size_t i = 0; i < vec.size(); i++)
			vec[i] *= DIGITAL_BASE;
		
		iQuotient = iDividend / DIGITAL_BASE;
		iRemainder = iDividend % DIGITAL_BASE;
		vec.push_back(iRemainder);

		iDividend = iQuotient;
	} while (iDividend > 0);
	
	return std::accumulate(vec.begin(), vec.end(), 0); // sum all the integer values in the vector.
}

static void inputInt(int &powInt)
{
	std::cout << "Please input an positive integer to get power result (0 - 99999): ";
	
	std::cin >> powInt;
	if (!std::cin)
	{
		std::cerr << "There is something wrong when getting your input!" << std::endl;
		exit(-1);
	}
}

int main()
{
#ifdef __DEV_ENV
	int powInt = 0;
	
	//require the user to input an positive integer
	do 
	{
		inputInt(powInt);
		if (powInt < 0)
			std::cout << "Only positive integers are accepted!" << std::endl;
	} while (powInt < 0); 

	int reversedInt = reverseInt(powInt);

	std::cout << "Result: " << powInt << '^' << reversedInt << " is " 
		<< enhancedPow(BigInt(powInt), BigInt(reversedInt)) << std::endl;

#endif // The main program will run in DEV environments

#ifndef __DEV_ENV
	cout << "This testing program will only run in DEV environments!" << endl;
#endif // print information for none-DEV environment
	return 0;
}