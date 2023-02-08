#include <iostream>
#include <cstdint>

#ifndef FRACTION_H
#define FRACTION_H

class Fraction {
	public:
		//Want intial vals to only be here once, should be in .h
		Fraction(int64_t n = 0, int64_t d=1);
		~Fraction() = default;
		
		
		Fraction operator+(Fraction rhs);
		Fraction operator-(Fraction rhs);
		Fraction operator*(Fraction rhs);
		Fraction operator/(Fraction rhs);
		Fraction &operator=(Fraction rhs);
		
		
		bool operator==(Fraction rhs);
		bool operator<=(Fraction rhs);
		bool operator>=(Fraction rhs);
		bool operator!=(Fraction rhs);
		bool operator<(Fraction rhs);
		bool operator>(Fraction rhs);
		
		
		int64_t getNum() {return num;}
		int64_t getDen() {return den;}
		
		
		
		
	private:
		int64_t
			num,
			den;
};

std::istream &operator>>(std::istream &, Fraction &);
std::ostream &operator<<(std::ostream &, Fraction);
#endif
