#include "fraction.h"
#include "point.h"
#include <iostream>
#include <cmath>

using namespace std;



const uint32_t
    MAX_POINTS = 100000;
   
//Global definition of order and tempOrder for merge y
uint32_t order[MAX_POINTS];
uint32_t tempOrder[MAX_POINTS];

Fraction naiveClosestPair(Point pList[], uint32_t, uint32_t &, uint32_t &);
Fraction fastClosestPair(Point pList[], uint32_t, uint32_t &, uint32_t &);
Fraction fastClosestPairRec(Point pList[], uint32_t, uint32_t, uint32_t &, uint32_t &);
//=============================================================================
// int compareX(const void *a,const void *b)
//   Compare points by x-coordinates, break ties by y-coordinates
//
// Parameters
//  a - pointer to a Point object
//  b - pointer to a Point object
//
// Returns
//  -1 if a < b
//  0 if a == b
//  1 if a > b
//

int compareX(const void *a,const void *b) {
    auto
        *pa = (Point *)a,
        *pb = (Point *)b;
    Fraction
        d = pa->getX() - pb->getX();	// difference in x-coordinates

    // if x-coordinates are different, return 1 or -1
    if (d < 0)
        return -1;
    if (d > 0)
        return 1;
        
    // if we get here, x-coordinates are the same, do the same testing
    // with the y-coordinates
    d = pa->getY() - pb->getY();
    if (d < 0)
        return -1;
    if (d > 0)
        return 1;
        
    // if we get here, the points are the same
    return 0;
}


//=============================================================================
// void sortPointsX(Point *pList,uint32_t nPoints)
//  sort an array of Point objects by x-coordinate
//
// Parameters
//  pList   - an array of Point objects
//  nPoints - the number of Point objects in the pList array
//
// Note:
// - this just uses the C library's qsort() function. This requires a callback
//   function to compare elements; that's the purpose of the compareX() function
//   above
//

void sortPointsX(Point *pList,uint32_t nPoints) {
    qsort(pList,nPoints,sizeof(Point),compareX);
}

//=============================================================================
// int main(int32_t argc,char *argv[])
//  the main function
//
// Parameters
//  argc - number of elements given on the command line. Should be 2.
//  argv - array of C strings holding the contents of the command line
//
// Returns
//  0 if the program runs successfully, 1 if not
//

int main(int32_t argc,char *argv[]) {
    Point
        pList[MAX_POINTS];
    uint32_t
        nPoints,
        p1,p2;
    Fraction
        minDistNaive,
        minDistFast;
    double
        d;

    // make sure there's an option (-n, -f or -b) given on the command line
    if (argc < 2 || argv[1][0] != '-') {
        cout << "Usage error" << endl;
        return 1;
    }

    // read the points
    cin >> nPoints;

    for (uint32_t i=0;i<nPoints;i++)
        cin >> pList[i];

    // -n = use the brute force / naive method
    if (argv[1][1] == 'n') {

        minDistNaive = naiveClosestPair(pList,nPoints,p1,p2);

        cout << "Minimum distance (squared): " << minDistNaive << endl;
        cout << "points: " << pList[p1] << ' ' << pList[p2] << endl;

        d = sqrt(minDistNaive.getNum()) / sqrt(minDistNaive.getDen());

        cout << "Distance: " << d << endl;

    // -f = use the fast / divide and conquer method
    } else if (argv[1][1] == 'f') {

        minDistFast = fastClosestPair(pList,nPoints,p1,p2);

        cout << "Minimum distance (squared): " << minDistFast << endl;
        cout << "points: " << pList[p1] << ' ' << pList[p2] << endl;

        d = sqrt(minDistFast.getNum()) / sqrt(minDistFast.getDen());

        cout << "Distance: " << d << endl;

    // -b = use both methods to compare answers
    } else if (argv[1][1] == 'b') {

        minDistNaive = naiveClosestPair(pList,nPoints,p1,p2);

        cout << "Brute force:" << endl;
        cout << "Minimum distance (squared): " << minDistNaive << endl;
        cout << "points: " << pList[p1] << ' ' << pList[p2] << endl;

        d = sqrt(minDistNaive.getNum()) / sqrt(minDistNaive.getDen());

        cout << "Distance: " << d << endl;

        cout << "\nFast:" << endl;
        minDistFast = fastClosestPair(pList,nPoints,p1,p2);

        cout << "Minimum distance (squared): " << minDistFast << endl;
        cout << "points: " << pList[p1] << ' ' << pList[p2] << endl;

        d = sqrt(minDistFast.getNum()) / sqrt(minDistFast.getDen());

        cout << "Distance: " << d << endl;

    } else {
        cout << "Usage error" << endl;
        return 1;
    }

    return 0;
}

Fraction naiveClosestPair(Point pList[], uint32_t nPoints, uint32_t &bestI, uint32_t &bestJ) {
	Fraction
		best(1, 0),
		d;
	Point
		distance;
	for(int i = 0; i < nPoints - 1; i++)
	{
		for(int j = i + 1; j < nPoints; j++)
		{
			d = distance.distance(pList[i], pList[j]);
			
			if (d < best)
			{
				best = d;
				bestI = i;
				bestJ = j;
			}
		}
	}
	
	return best;
}


Fraction fastClosestPair(Point pList[], uint32_t nPoints, uint32_t &bestI, uint32_t &bestJ) {
	Fraction
		best;
	sortPointsX(pList, nPoints);
	
	best = fastClosestPairRec(pList, 0, nPoints - 1, bestI, bestJ);

	return best;
}

Fraction fastClosestPairRec(Point pList[], uint32_t first, uint32_t last, uint32_t &bestI, uint32_t &bestJ)
{
	//Defines fraction d1 and d2 for distance comparison
	Fraction 
		best(1, 0),
		d1,
		d2,
		dfinal;
	//Defines separate bestI and bestJ for recursion and order array for storing y order from position in array
	uint32_t
		mid = 0,
		bestI1,
		bestJ1,
		bestI2,
		bestJ2;
	
	//Allows use of distance method
	Point
		distance;
	if(first == last)
	{
		//Will sort y coords by distance as x is already sorted
		order[first] = first;
		bestI = first;
		bestJ = first;
		return best;
	}
	
	mid = (first + last) / 2;
	d1 = fastClosestPairRec(pList, first, mid, bestI1, bestJ1);
	d2 = fastClosestPairRec(pList, mid + 1, last, bestI2, bestJ2);
	
	if (d2 < d1)
	{
		best = d2;
		bestI = bestI2;
		bestJ = bestJ2;
	} else {
		best = d1;
		bestI = bestI1;
		bestJ = bestJ1;
	}
	
	//Algorithm 4
	uint32_t
		left = first,
		right = mid + 1,
		k = first;
	while (left <= mid && right <= last)
	{
		if ((pList[order[right]].getY() < pList[order[left]].getY()) || 	//Choose between two points with smallest y coord
			((pList[order[right]].getY() == pList[order[left]].getY()) && //If tie of y, choose lowest x. If both tie choose left by default.
			(pList[order[right]].getX() < pList[order[left]].getX())))
			{
				//If right is larger, put in tempOrder[k] and check next right
				tempOrder[k] = order[right];
				right += 1;
			} else {
				//If left larger, put in tempOrder[k] and check next left
				tempOrder[k] = order[left];
				left += 1;
			}
			k += 1;
	}
	//Copy remaining points in list
	while (left <= mid) 
	{
		tempOrder[k] = order[left];
		left += 1;
		k += 1;
	}
	while (right <= last)
	{
		tempOrder[k] = order[right];
		right += 1;
		k += 1;
	}
	left = first;
	for(k = first; k <= last;k++) {
		order[k] = tempOrder[k];
		//Remember points close to median
		if ( (pList[order[k]].getX() - pList[mid].getX()) * (pList[order[k]].getX() - pList[mid].getX()) <= best) {
			tempOrder[left] = order[k];
			left += 1;
		}
	}
	
	//Algorithm 5
	for(k = first; k < left; k++)
	{
		for(right = k + 1; right < left; right++)
		{
			if((pList[tempOrder[k]].getY() - pList[tempOrder[right]].getY()) * (pList[tempOrder[k]].getY() - pList[tempOrder[right]].getY()) > best)
			{
			
				break;
			}
			//Uses distance function provided by point distance variable method
			dfinal = distance.distance(pList[tempOrder[k]], pList[tempOrder[right]]);
			if (dfinal < best)
			{
				best = dfinal;
				bestI = tempOrder[k];
				bestJ = tempOrder[right];
			}
		}
	}
	return best;
}
