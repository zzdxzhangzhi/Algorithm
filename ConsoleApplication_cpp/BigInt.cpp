/**
* @file BigInt.cpp
* @author Zhang Zhi
* @brief the implementation of BigInt class
* @since 1.0.0
*
* Change Log:
*
*/


#include "BigInt.h"
#include <iostream>
#include <iterator>
#include <iomanip>

/**
* @brief output operator
*
* using any output stream to output the integer data
*/
std::ostream& operator <<(std::ostream &out, BigInt &a_bigInt)
{	
	if (!a_bigInt.m_isPositive)
		out << SIGN_NEGATIVE;

	// every section will occupy 9 bits and using '0' to fill out the blank
	std::list<int>::const_iterator iter = a_bigInt.m_intData.cbegin();
	for(int i = 0; iter != a_bigInt.m_intData.cend(); iter++, i++)
	{
		if (i > 0)
		{
			out << std::setw(INT_SECTION_WIDTH)
				<< std::setfill(FILL_CHAR_OUTPUT_FOR_INT_SECTION)
				<< *iter;
		}
		else
		{
			out << *iter; // the first section will not fill out with '0'
		}
	}

	out << std::endl;

	return out;
}

/**
* @brief plus-equal operator
*
* result will plus the 2 integers that the 2 BigInt objects hold
*/
BigInt& BigInt::operator +=(const BigInt &ex_bigInt)
{
	if (m_isPositive == ex_bigInt.m_isPositive)
	{
		m_AbsPlus(ex_bigInt);
	}
	else if (m_isPositive)
	{
		m_AbsMinus(ex_bigInt);
		m_isPositive = !m_isAbsLittleThan(ex_bigInt);
	}
	else
	{
		m_AbsMinus(ex_bigInt);
		m_isPositive = m_isAbsLittleThan(ex_bigInt);
	}

	return *this;
}

/**
* @brief function for multiple
*
* result will multiple the BigInt with an Integer
*/
BigInt multipleWithInt(const BigInt &bi_multiplicand, int ex_multiplier)
{
	BigInt t_res;

	std::list<int>::const_reverse_iterator iter = bi_multiplicand.m_intData.crbegin();
	long long t_multiple = 0;
	int t_quotient = 0;
	for (; iter != bi_multiplicand.m_intData.crend(); iter++)
	{
		t_multiple = static_cast<long long>(*iter) * ex_multiplier + t_quotient;
		t_res.m_intData.push_front(static_cast<int>(t_multiple % (MAX_9BIT_INT_VALUE + 1)));

		//quotient will be added to the multiple results of previous section
		t_quotient = static_cast<int>(t_multiple / (MAX_9BIT_INT_VALUE + 1));

		//if there is no section in front of the list, add a new one.
		std::list<int>::const_reverse_iterator t_iter = iter;
		if (t_quotient > 0 && (bi_multiplicand.m_intData.crend() == (++t_iter)))
			t_res.m_intData.push_front(t_quotient);
	}

	return t_res;
}

/**
* @brief multiple-equal operator
*
* result will multiple the 2 integers that the 2 BigInt objects hold
*/
BigInt& BigInt::operator *=(const BigInt &ex_bigInt)
{
	m_isPositive = m_isPositive && ex_bigInt.m_isPositive;

	BigInt t_res(0);
	BigInt t_multiplicand(m_intData);

	std::list<int>::const_reverse_iterator ex_iter = ex_bigInt.m_intData.crbegin();
	for (; ex_iter != ex_bigInt.m_intData.crend(); ex_iter++)
	{
		t_res += multipleWithInt(t_multiplicand, *ex_iter);
		t_multiplicand.m_intData.push_back(0); // the tail section need to be set as 0, equivalent to multiple 1000000000
	}

	return (*this = t_res);
}

/**
* @brief power function for BigInt
*
* return the power value as assigned exp
*/
BigInt BigInt::pow(const BigInt &exp) const
{
	if (exp < 0)
	{
		std::cout << "exponent should not be negative!" << std::endl;
		return 0;
	}

	BigInt t_res(1);
	for (BigInt i = 0; i < exp; i++)
	{
		t_res *= *this;
	}

	return t_res;
}

/**
* @brief transform long long value to BigInt
*
* transform the long long value to BigInt
*/
void BigInt::m_transformLL(long long intValue)
{
	if (intValue <= MAX_9BIT_INT_VALUE)
	{
		m_intData.push_back(static_cast<int>(intValue));
	}
	else
	{
		long long t_dividend = intValue;
		while (t_dividend > MAX_9BIT_INT_VALUE)
		{
			int t_quotient = static_cast<int>(t_dividend / (MAX_9BIT_INT_VALUE + 1));
			int t_remainder = static_cast<int>(intValue % (MAX_9BIT_INT_VALUE + 1));
			m_intData.push_front(t_remainder);
			if (t_quotient <= MAX_9BIT_INT_VALUE)
				m_intData.push_front(t_quotient);
			else
				t_dividend = t_quotient;
		}
	}
}

/**
* @brief contrast Abs if it is little
*
* When the BigInt Abs value is less than the other one, return true
*/
bool BigInt::m_isAbsLittleThan(const BigInt &ex_bigInt) const
{
	if (m_intData.size() < ex_bigInt.m_intData.size())
		return true;

	if (m_intData.size() > ex_bigInt.m_intData.size())
		return false;

	std::list<int>::const_iterator m_iter = m_intData.cbegin();
	std::list<int>::const_iterator ex_iter = ex_bigInt.m_intData.cbegin();
	for (; m_iter != m_intData.cend(); m_iter++, ex_iter++)
	{
		if (*m_iter < *ex_iter)
			return true;

		if (*m_iter > *ex_iter)
			return false;
	}

	return false;
}

/**
* @brief contrast Abs if the 2 BigInt are equal
*
* When the BigInt Abs value is equal to the other one, return true
*/
bool BigInt::m_isAbsEqualTo(const BigInt &ex_bigInt) const
{
	if (m_intData.size() != ex_bigInt.m_intData.size())
		return false;

	std::list<int>::const_iterator m_iter = m_intData.cbegin();
	std::list<int>::const_iterator ex_iter = ex_bigInt.m_intData.cbegin();
	for (; m_iter != m_intData.cend(); m_iter++, ex_iter++)
	{
		if (*m_iter != *ex_iter)
			return false;
	}

	return true;
}

/**
* @brief plus 2 BigInt by Abs value
*
* plus the Abs value for 2 BigInt
*/
void BigInt::m_AbsPlus(const BigInt &ex_bigInt)
{
	std::list<int>::reverse_iterator m_iter = m_intData.rbegin();
	std::list<int>::const_reverse_iterator ex_iter = ex_bigInt.m_intData.crbegin();

	for (int t_quotient = 0;
		m_iter != m_intData.rend() && ex_iter != ex_bigInt.m_intData.crend();
		m_iter++, ex_iter++)
	{
		// Plus example:
		// 999999999 + 2 = 1000000001
		// Then there should only left 1 to the last section 
		// and add 1 to the reversed second section
		int t_res = *m_iter + *ex_iter + t_quotient;
		t_quotient = t_res / (MAX_9BIT_INT_VALUE + 1);
		*m_iter = t_res % (MAX_9BIT_INT_VALUE + 1);

		// When the first BigInt has no left section, add new one for it
		std::list<int>::reverse_iterator t_iter = m_iter;
		if ((m_intData.rend() == (++t_iter)) && (t_quotient > 0))
			m_intData.push_front(t_quotient);
	}

	// if the other BigInt still has more sections of value, then add them to the first BigInt directly
	for (; ex_iter != ex_bigInt.m_intData.crend(); ex_iter++)
	{
		m_intData.push_front(*ex_iter);
	}
}

/**
* @brief minus this BigInt by the external BigInt
*
* minus the Abs value for 2 BigInt
*/
void BigInt::m_AbsMinus(const BigInt &ex_bigInt)
{
	std::list<int> t_intValue;

	//Swap the minuend if it is smaller than the substractor
	if (m_isAbsLittleThan(ex_bigInt))
	{
		t_intValue = m_intData;
		m_intData = ex_bigInt.m_intData;
	}
	else
		t_intValue = ex_bigInt.m_intData;

	std::list<int>::reverse_iterator m_iter = m_intData.rbegin();
	std::list<int>::const_reverse_iterator t_iter = t_intValue.crbegin();

	for (int t_quotient = 0;
		m_iter != m_intData.rend() && t_iter != t_intValue.crend();
		m_iter++, t_iter++)
	{
		//brow MAX_9BIT_INT_VALUE + 1 from the previous section
		if (*m_iter < *t_iter)
		{
			std::list<int>::reverse_iterator t_iter = m_iter;
			--(*(++t_iter));
			*m_iter += (MAX_9BIT_INT_VALUE + 1);
		}

		*m_iter -= *t_iter;
	}

	//clear the zero sections
	if (m_iter == m_intData.rend())
	{
		for (; m_iter != m_intData.rbegin(); m_iter = m_intData.rend())
		{
			if (0 == *(--m_iter))
				m_intData.pop_front();
			else
				break;
		}
	}
}