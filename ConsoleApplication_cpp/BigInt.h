/**
* @file BigInt.h
* @author Zhang Zhi
* @brief the interface of BigInt class
* @since 1.0.0
*
* Change Log:
*
*/


#ifndef __BIGINT_H
#define __BIGINT_H


#include <list>
#include <algorithm>
#include <iostream>
#include <limits>
#include <iterator>


const int MAX_9BIT_INT_VALUE = 999999999;
constexpr long long MAX_LLONG_VALUE = std::numeric_limits<long long>::max();
constexpr unsigned long long MAX_ULLONG_VALUE = std::numeric_limits<unsigned long long>::max();
const char SIGN_NEGATIVE = '-';
const int INT_SECTION_WIDTH = 9;
const char FILL_CHAR_OUTPUT_FOR_INT_SECTION = '0';

/**
* @brief this class define the Big Integer
*
*/
class BigInt
{
public:
	/**
	* @brief constructor
	*
	* initialize a empty BigInt object
	*/
	BigInt()
		:m_isPositive(true)
	{
	}

	/**
	* @brief constructor
	*
	* initialize a BigInt object with a Long Long Integer
	*/
	BigInt(long long intValue)
		:m_isPositive(intValue >= 0)
	{
		long long positiveIntValue = m_isPositive ? intValue : (-1) * intValue;
		m_transformLL(positiveIntValue);
	}

	/**
	* @brief constructor
	*
	* initialize a BigInt object with a list of integer
	*/
	BigInt(const std::list<int> &ex_intData, bool ex_isPositive = true)
		:m_intData(ex_intData), m_isPositive(ex_isPositive)
	{
	}

	/**
	* @brief copy constructor
	*
	* initialize a BigInt object with another BigInt object
	*/
	BigInt(const BigInt &) = default;

	BigInt(BigInt &&) = default;

	/**
	* @brief assign operator
	*
	* assign value to a BigInt object with another BigInt object
	*/
	BigInt& operator =(const BigInt &) = default;
	
	BigInt& operator =(BigInt &&) = default;

	/**
	* @brief output operator
	*
	* using any output stream to output the integer data
	*/
	friend std::ostream& operator <<(std::ostream &out, BigInt &a_bigInt);

	/**
	* @brief plus operator
	*
	* result will plus the 2 integers that the 2 BigInt objects hold
	*/
	friend BigInt operator +(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		BigInt t_res = a_bigInt;
		return (t_res += b_bigInt); 
	}

	/**
	* @brief plus-equal operator
	*
	* result will plus the 2 integers that the 2 BigInt objects hold
	*/
	BigInt& operator +=(const BigInt &ex_bigInt);

	/**
	* @brief minus operator
	*
	* result will minus the 2 integers that the 2 BigInt objects hold
	*/
	friend BigInt operator -(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		BigInt t_res = a_bigInt;
		return (t_res -= b_bigInt);
	}

	/**
	* @brief minus-equal operator
	*
	* result will minus the 2 integers that the 2 BigInt objects hold
	*/
	BigInt& operator -=(const BigInt &ex_bigInt)
	{
		BigInt t_res(ex_bigInt.m_intData, !ex_bigInt.m_isPositive);
		return (*this += t_res);
	}

	/**
	* @brief function for multiple
	*
	* result will multiple the BigInt with an Integer
	*/
	friend BigInt multipleWithInt(const BigInt &bi_multiplicand, int ex_multiplier);
	
	/**
	* @brief multiple operator
	*
	* result will multiple the 2 integers that the 2 BigInt objects hold
	*/
	friend BigInt operator *(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		BigInt t_res = a_bigInt;
		return (t_res *= b_bigInt);
	}

	/**
	* @brief multiple-equal operator
	*
	* result will multiple the 2 integers that the 2 BigInt objects hold
	*/
	BigInt& operator *=(const BigInt &ex_bigInt);
	
	/**
	* @brief self-increase operator
	*
	* result will plus integers that the BigInt objects hold with 1
	*/
	BigInt& operator ++()
	{
		return (*this += 1);
	}

	/**
	* @brief self-increase operator suffix
	*
	* result will plus integers that the BigInt objects hold with 1
	*/
	BigInt operator ++(int)
	{
		BigInt t_res(*this);
		++*this;

		return t_res;
	}

	/**
	* @brief self-decrease operator
	*
	* result will minus the integers that the BigInt objects hold with 1
	*/
	BigInt& operator --()
	{
		return (*this -= 1);
	}

	/**
	* @brief self-decrease operator suffix
	*
	* result will minus the integers that the BigInt objects hold with 1
	*/
	BigInt& operator --(int)
	{
		BigInt t_res(*this);
		--*this;

		return t_res;
	}

	/**
	* @brief less-than operator
	*
	* if a_bigInt value is less than b_bigInt, return true
	*/
	friend bool operator <(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		if (!a_bigInt.m_isPositive && b_bigInt.m_isPositive)
			return true;

		if (a_bigInt.m_isPositive && !b_bigInt.m_isPositive)
			return false;

		return (a_bigInt.m_isPositive ? 
			a_bigInt.m_isAbsLittleThan(b_bigInt) : !a_bigInt.m_isAbsLittleThan(b_bigInt));
	}

	/**
	* @brief great-than operator
	*
	* if a_bigInt value is great than b_bigInt, return true
	*/
	friend bool operator >(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		return !(a_bigInt <= b_bigInt);
	}

	/**
	* @brief equivalent operator
	*
	* if a_bigInt value is equal with b_bigInt, return true
	*/
	friend bool operator ==(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		if (a_bigInt.m_isPositive != b_bigInt.m_isPositive)
			return false;

		return a_bigInt.m_isAbsEqualTo(b_bigInt);
	}

	/**
	* @brief none-equivalent operator
	*
	* if a_bigInt value is not equal with b_bigInt, return true
	*/
	friend bool operator !=(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		return !(a_bigInt == b_bigInt);
	}

	/**
	* @brief less-than and equal operator
	*
	* if a_bigInt value is less than or equal to b_bigInt, return true
	*/
	friend bool operator <=(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		return (a_bigInt < b_bigInt) && (a_bigInt == b_bigInt);
	}

	/**
	* @brief great-than and equal operator
	*
	* if a_bigInt value is greater than or equal to b_bigInt, return true
	*/
	friend bool operator >=(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		return !(a_bigInt < b_bigInt);
	}

	/**
	* @brief power function for BigInt
	*
	* return the power value as assigned exp
	*/
	BigInt pow(const BigInt &exp) const;
	
	/**
	* @brief power function for  ASZXCV S 2 BigInt
	*
	* return the power value, b_bigInt is the exponent
	*/
	friend BigInt enhancedPow(const BigInt &a_bigInt, const BigInt &b_bigInt)
	{
		return a_bigInt.pow(b_bigInt);
	}

private:
	std::list<int> m_intData;
	bool m_isPositive; // indicate the BigInt is positive or not

	/**
	* @brief transform long long value to BigInt
	*
	* transform the long long value to BigInt
	*/
	void m_transformLL(long long intValue);

	/**
	* @brief contrast Abs if it is little
	*
	* When the BigInt Abs value is less than the other one, return true
	*/
	bool m_isAbsLittleThan(const BigInt &ex_bigInt) const;

	/**
	* @brief contrast Abs if the 2 BigInt are equal
	*
	* When the BigInt Abs value is equal to the other one, return true
	*/
	bool m_isAbsEqualTo(const BigInt &ex_bigInt) const;

	/**
	* @brief plus 2 BigInt by Abs value
	*
	* plus the Abs value for 2 BigInt
	*/
	void m_AbsPlus(const BigInt &ex_bigInt);

	/**
	* @brief minus this BigInt by the external BigInt
	*
	* minus the Abs value for 2 BigInt
	*/
	void m_AbsMinus(const BigInt &ex_bigInt);
};

#endif //__BIGINT_H
