#include <sstream>
#include <iostream>
#include "point.h"

using namespace std;

int main ()
{
	Fraction
		zero,
		x1(0, 1),
		x2(4, 1),
		x3(16, 1),
		y1(0, 1),
		y2(4, 1),
		y3(16, 4);
	Point
		a(x1, y1),
		b(x2),
		c,
		p1(x3, y1),
		p2(x2, y1);
	bool okay = true;
	//Constructor test
	if(a.getX() != x1 || a.getY() != y1)
	{
		cout << "Error two param\n";
		cout << "Value of point a: " << a << endl;
		cout << "Expected: " << x1 << ' ' << y1 << endl;
		okay = false;
	}
	if(b.getX() != x2 || b.getY() != zero)
	{
		cout << "Error one param\n";
		cout << "Value of point b: " << b << endl;
		cout << "Expected: " << x2 << ' ' << zero << endl;
		okay = false;
	}
	if(c.getX() != zero || c.getY() != zero)
	{
		cout << "Error no param\n";
		cout << "Value of point c: " << c << endl;
		cout << "Expected: " << zero << ' ' << zero << endl;
		okay = false;
	}
	cout << "Constructor: " << (okay ? "pass" : "fail") << endl;
	
	//Distance test
	Point p;
	Fraction d = p.distance(p1, p2);
	if(d != Fraction(144, 1))
	{
		cout << "Error distance wrong\n";
		cout << "Value of distance: " << d << endl;
		cout << "Expected: 144/1" << endl;
		okay = false;	
	}
	
	cout << "Distance: " << (okay ? "pass" : "fail") << endl;
	
	
	//Assignment test
	b = a;
	if(b.getX() != a.getX() || b.getY() != a.getY())
	{
		cout << "Assignment error\n";
		cout << "B value: " << b << ". Expected: " << a << endl;
		okay = false;
	}
	
	cout << "Assignment: " << (okay ? "pass" : "fail") << endl;
}
