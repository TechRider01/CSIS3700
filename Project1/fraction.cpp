#include "fraction.h"

/* 		
		dFraction(int32_t n = 0, int32_t d=1);
		
		dFraction Fraction::operator+(Fraction rhs);
		dFraction Fraction::operator-(Fraction rhs);
		dFraction Fraction::operator*(Fraction rhs);
		dFraction Fraction::operator/(Fraction rhs);
		dFraction &Fraction::operator=(Fraction rhs);
		
		dbool Fraction::operator==(Fraction rhs);
		dbool Fraction::operator<=(Fraction rhs);
		dbool Fraction::operator>=(Fraction rhs);
		dbool Fraction::operator!=(Fraction rhs);
		dbool Fraction::operator<(Fraction rhs);
		dbool Fraction::operator>(Fraction rhs);
		
		dint32_t Fraction::getNum() {return num;}
		dint32_t Fraction::getDen() {return den;}
		
		dstd::istream &operator>>(std::istream &, Fraction &);
		dstd::ostream &operator<<(std::ostream &, Fraction);
*/
	//When a function is static the function can only be seen in this file
	static int64_t gcd(int64_t a, int64_t b)
	{
		int64_t 
			r;
		a = (a < 0) ? -a : a;
		b = (b < 0) ? -b : b;
		while(b != 0)
		{
			r = a % b;
			a = b;
			b = r;
		}
	return a;
	}
	
	//Must put class name in front of function call
	Fraction::Fraction(int64_t n, int64_t d) {
		if(d < 0) {
			n = -n;
			d = -d;
		}
		int64_t 
			g = gcd(n,d);
		
		num = n/g;
		den = d/g;
	}
	
	Fraction Fraction::operator+(Fraction rhs) {
		// (a * d + b * d) / b* d
		// New fraction and let constructor reduce to lowest terms
		int64_t 
			s,
			t;
		s = num * rhs.den + den * rhs.num;
		t = den * rhs.den;
		
		return Fraction(s,t);
	}
	
	Fraction Fraction::operator-(Fraction rhs){
		// (a * d + b * d) / b* d
		// New fraction and let constructor reduce to lowest terms
		int64_t 
			s,
			t;
		s = num * rhs.den - den * rhs.num;
		t = den * rhs.den;
		
		return Fraction(s,t);
	}
	
	Fraction Fraction::operator*(Fraction rhs) {
		// (a * d + b * d) / b* d
		// New fraction and let constructor reduce to lowest terms
		int64_t 
			s,
			t;
		s = num * rhs.num;
		t = den * rhs.den;
		
		return Fraction(s,t);
	}
	
	Fraction Fraction::operator/(Fraction rhs) {
		// (a * d + b * d) / b* d
		// New fraction and let constructor reduce to lowest terms
		int64_t 
			s,
			t;
		s = num * rhs.den;
		t = den * rhs.num;
		
		return Fraction(s,t);
	}
	
	Fraction &Fraction::operator=(Fraction rhs) {
	
		num = rhs.num;
		den = rhs.den;
		return *this;
	}
	bool Fraction::operator==(Fraction rhs) {
		return num == rhs.num && den == rhs.den;
	}
	
	bool Fraction::operator!=(Fraction rhs) {
		return num != rhs.num || den != rhs.den;
	}
	
	bool Fraction::operator<(Fraction rhs) {
		return num * rhs.den < den * rhs.num;
	}
	
	bool Fraction::operator>(Fraction rhs) {
		return num * rhs.den > den * rhs.num;
	}
	
	bool Fraction::operator<=(Fraction rhs) {
		return num * rhs.den <= den * rhs.num;
	}
	
	bool Fraction::operator>=(Fraction rhs) {
		return num * rhs.den >= den * rhs.num;
	}
	
	std::istream &operator>>(std::istream &is, Fraction &f) {
		int64_t
			n,d;
		char
			slash;
		is >> n >> slash >> d;
		f = Fraction(n,d);
		
		return is;
	}
	std::ostream &operator<<(std::ostream &os, Fraction f) {
		os << f.getNum() << " / " << f.getDen();
		return os;
	}
