// -----------------------------------------------------------
//
// This file is part of kortex
//
// Copyright (C) 2013 Engin Tola
//
// All rights reserved.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
//
// -----------------------------------------------------------

#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>

#include <kortex/fileio.h>
#include <kortex/random.h>

#include "gui.h"

using namespace std;
using namespace kortex;

void display_help() {
    printf("usage\n");
    printf("  -l line_file   |\n");
    printf("  -p point_file  |\n");
    printf("  -g             | enable gui\n");
}

struct Options {
    string ifile;
    string ofile;

    string line_file;
    string point_file;

    bool   gui;
    Options() {
        ifile = "";
        ofile = "";

        point_file = "";
        line_file  = "";

        gui   = true;
    }
};

int main(int argc, char **argv) {

    if( argc == 1 ) {
       display_help();
       exit(0);
    }

    Options opts;

    int counter=1;
    while( counter < argc ) {
        if( !strcmp("-i", argv[counter] ) ) {
            counter++;
            opts.ifile = argv[counter++];
            continue;
        }

        if( !strcmp("-l", argv[counter] ) ) {
            counter++;
            opts.line_file = argv[counter++];
            continue;
        }
        if( !strcmp("-p", argv[counter] ) ) {
            counter++;
            opts.point_file = argv[counter++];
            continue;
        }

        if( !strcmp("-o", argv[counter] ) ) {
            counter++;
            opts.ofile = argv[counter++];
            continue;
        }
        if( !strcmp("-g", argv[counter] ) ) {
            counter++;
            opts.gui = true;
            continue;
        }
        printf("unkown option [%s]\n", argv[counter]);
        exit(1);
    }

    if( opts.ofile=="" ) opts.ofile="out";

    vector<Line2d> lines;
    if( opts.line_file != "" ) {
        ifstream fin;
        open_or_fail( opts.line_file, fin );
        int np = 0;
        fin >> np;
        for( int i=0; i<np; i++ ) {
            double a, b, c;
            fin >> a >> b >> c;
            Line2d line(a,b,c);
            lines.push_back( line );
        }
        fin.close();
    }

    vector<Point2d> points;
    if( opts.point_file != "" ) {
        ifstream fin;
        open_or_fail( opts.point_file, fin );
        int np = 0;
        fin >> np;
        for( int i=0; i<np; i++ ) {
            double a, b;
            fin >> a >> b;
            Point2d pnt(a,b);
            points.push_back( pnt );
        }
        fin.close();
    }


    if( opts.gui ) {

        int w = 501;
        int h = 501;

        if( 0 ) {
        initialize_random_seed();
        vector<Point2d> pnts;
        for( int i=0; i<100; i++ ){
            Point2d pnt;
            pnt.x = double(rand())/RAND_MAX * w;
            pnt.y = double(rand())/RAND_MAX * h;
            pnts.push_back(pnt);
        }
        }
        GUI gui;
        gui.setup( &points, &lines );
        gui.create(w,h);
        gui.display();
    }

}

// Local Variables:
// mode: c++
// compile-command: "make -C ."
// End:
