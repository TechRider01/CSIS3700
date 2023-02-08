/*
		//Constructor destructor
		
		~Point() = default;
		
		//Assignment overload for point
		Point &operator=(Point rhs);
		
		//Distance calculator
		Point distance(Fraction x1, Fraction y1, Fraction x2, Fraction y2);
		
		//Get val of x and y
		int64_t getX() {return x;}
		int64_t getY() {return y;}
		std::istream &operator>>(std::istream &, Point &);
		std::ostream &operator<<(std::ostream &, Point &);
*/

#include "point.h"
	
	//Takes 4 ints, assigns first two to x, second to to y as fraction objects
	Point::Point(Fraction x, Fraction y) {
		this->x = x;
		this->y = y;
	}
	
	Fraction Point::distance(Point p1, Point p2) {
		return ((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)); 
	}
	
	Point &Point::operator=(Point rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	
	std::istream &operator>>(std::istream &is, Point &p) {
		Fraction
			x, y;
		char
			leftBrace,
			comma,
			rightBrace;
			
		is >> leftBrace >> x >> comma >> y >> rightBrace;
		p = Point(x, y);
		
		return is;
	}
	std::ostream &operator<<(std::ostream &os, Point p) {
		os << "(" << p.getX() << "," << p.getY() << ")";
		return os;
	}
