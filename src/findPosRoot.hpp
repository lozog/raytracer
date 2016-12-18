#pragma once

#include <cmath>                            // pow
#include <algorithm>                        // swap
#include "polyroots.hpp"                    // quadraticRoots
#include <iostream>

size_t quadraticRoots(double A, double B, double C, double roots[2]);

/*
	Solves a*t^2 + b*t + c, stores minimum positive root in t0
	if such a root exists
*/
bool findPosRoot(double a, double b, double c, double & t0) {
		// std::cout << a << " " << b << " " << c << std::endl;
    double t1;

    double roots[2];
    size_t numRoots = quadraticRoots(a, b, c, roots);

    t0 = roots[0];
    t1 = roots[1];

    if (numRoots == 2) {
        // std::cout << "t0: " << t0 << ", t1: " << t1 << std::endl;
        if ( t0 > t1 ) std::swap(t0, t1);

		if ( t0 < 0 ) {
			t0 = t1;								// t0 negative, so try t1
        	if ( t0 < 0 )
				return false;						// t0 and t1 both negative
		} // if

    } else if ( numRoots == 1 ) {
        // std::cout << "det == 0" << std::endl;
        if ( t0 < 0 ) {
        	return false;
        }
    } else {
        return false;								// no real roots
    } // if
    // std::cout << "found: " << t0 << std::endl;
	return true;
}
