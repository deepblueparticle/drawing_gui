// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_STRUCTS_H
#define KORTEX_STRUCTS_H

#include <cmath>
#include <kortex/math.h>

namespace kortex {

    struct Point2d {
        double x;
        double y;
        Point2d() {
            x = 0.0;
            y = 0.0;
        }
        Point2d(double x_, double y_) {
            x = x_;
            y = y_;
        }
    };

    struct Circle2d {
        double cx, cy;
        double r;
        Circle2d() {
            cx = cy = 0.0;
            r = 1.0;
        }
        Circle2d( double x0, double y0, double rad ) {
            cx = x0;
            cy = y0;
            r  = rad;
        }
    };

    struct Segment2d {
        double x0, y0;
        double x1, y1;
        Segment2d() { x0 = y0 = x1 = y1 = 0.0; }
        Segment2d( double x0_, double y0_, double x1_, double y1_ ) {
            x0 = x0_;
            y0 = y0_;
            x1 = x1_;
            y1 = y1_;
        }
    };

    /// line equation ax + by + c = 0
    struct Line2d {
        double a, b, c;

        Line2d() {
            a = b = c = 0.0;
        }
        Line2d( double a_, double b_, double c_ ) {
            a = a_;
            b = b_;
            c = c_;
        }

        Line2d( double x0, double y0, double x1, double y1 ) {
            if( fabs( x1-x0 ) > 1e-20 ) {
                a = -( y1-y0 ) / (x1-x0 );
                b = 1.0;
                c = -a*x0 - b*y0;
            } else {
                a = 1.0;
                b = 0.0;
                c = -x0;
            }
        }

        bool is_vertical() const {
            return (b==0.0);
        }

        double y_intercept( int x0 ) const {
            if( b != 0.0 ) {
                return -(a*x0+c)/b;
            } else {
                return 1e30;
            }
        }

        double x_intercept( int y0 ) const {
            return -(b*y0+c) / a;
        }

    };



}

#endif
