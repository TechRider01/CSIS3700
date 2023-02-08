#include "fraction.h"

#ifndef POINT_H
#define POINT_H

class Point {
	private:
		Fraction 
			x,
			y;
	public:
		//Constructor destructor
		Point(Fraction x = 0, Fraction y = 0);
		~Point() = default;
		
		//Assignment overload for point
		Point &operator=(Point rhs);
		
		//Distance calculator
		Fraction distance(Point p1, Point p2);
		
		//Get val of x and y
		Fraction getX() {return x;}
		Fraction getY() {return y;}
	};
	
//Input output overloads
std::istream &operator>>(std::istream &, Point &);
std::ostream &operator<<(std::ostream &, Point);
#endif
