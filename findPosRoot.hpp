#pragma once

#include <cmath>                            // pow
#include <algorithm>                        // swap
#include "A4.hpp"                           // EPSILON
#include <iostream>

/*
	Solves a*t^2 + b*t + c, stores minimum positive root in t0
	if such a root exists
*/
// TODO: maybe we should be using polyroots.cpp here...
bool findPosRoot(float a, float b, float c, float & t0) {
		// std::cout << a << " " << b << " " << c << std::endl;
    float determinant = b*b - 4*a*c;
    float t1;
		// std::cout << determinant << std::endl;

    if (determinant > 0) {
        t0 = (-b + sqrt(determinant)) / (2*a);
        t1 = (-b - sqrt(determinant)) / (2*a);
        // std::cout << "t0: " << t0 << ", t1: " << t1 << std::endl;
        if ( t0 > t1 ) std::swap(t0, t1);

		if ( t0 < 0 ) {
			t0 = t1;								// t0 negative, so try t1
        	if ( t0 < 0 )
				return false;						// t0 and t1 both negative
		} // if

    } else if ( abs(determinant - 0.0f) < EPSILON) { // TODO: actually check properly
        std::cout << "det == 0" << std::endl;
        t0 = (-b + sqrt(determinant)) / (2*a);		// just one root
        if ( t0 < 0 ) {
        	return false;
        }
    } else {
        return false;								// no real roots
    } // if
    // std::cout << "found: " << t0 << std::endl;
	return true;
} // findPosRoot
