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
#include "gui.h"
#include <kortex/defs.h>
#include <kortex/fileio.h>
#include <kortex/string.h>

using namespace std;

GUI::GUI() {
    gx = 0;
    gy = 0;
    m_pnts  = NULL;
    m_lines = NULL;

    m_zsz = 101;
    m_b_enable_shadow = true;
    m_b_enable_help   = true;

    m_verbose = true;

    l0  = true;
    lx0 = 0;
    ly0 = 0;

    wzoom = NULL;
}

void GUI::create(int w, int h) {
    gw = w;
    gh = h;
    wmain.set_name("main window");
    wmain.create_display( w, h );
    wmain.create(0);
    wmain.resize(w,h);
    wmain.move(0,0);
    wmain.init_mouse();
    wmain.show();
}

void GUI::toggle_zoom_window() {
    if( wzoom ) {
        wzoom->destroy();
        delete wzoom;
        wzoom = NULL;
        return;
    }

    wzoom = new GUIWindow();
    wzoom->set_name("zoom");
    wzoom->create_display( m_zsz, m_zsz );
    wzoom->create(0);
    wzoom->resize( 3*m_zsz, 3*m_zsz );
    wzoom->move( gw, 0 );
    wzoom->show();
}

void GUI::setup( const vector<Point2d>* pnts, const vector<Line2d>* lines ) {
    m_pnts  = pnts;
    m_lines = lines;
}

void GUI::set_active_mode( DrawMode mode ) {
    m_dmode = mode;
    l0    = true;
    lx0   = 0;
    ly0   = 0;

    switch( mode ) {
    case DM_NONE   : printf("draw: disabled\n");     break;
    case DM_POINT  : printf("draw: point\n");        break;
    case DM_LINE   : printf("draw: line\n");         break;
    case DM_SEGMENT: printf("draw: line segment\n"); break;
    case DM_CIRCLE : printf("draw: circle\n");       break;
    }
}

void GUI::reset_mouse() {
    wmain.reset_mouse();
}

void GUI::catch_keyboard() {
    int c = wmain.wait(5);
    if     ( c == 'q' ) exit(1);
    else if( c == 'z' ) toggle_zoom_window();
    else if( c == 'v' ) m_verbose = !m_verbose;
    else if( c == 'm' ) m_b_enable_shadow = !m_b_enable_shadow;
    else if( c == 'h' ) m_b_enable_help = !m_b_enable_help;
    else if( c == 'p' ) set_active_mode( DM_POINT   );
    else if( c == 'c' ) set_active_mode( DM_CIRCLE  );
    else if( c == 'l' ) set_active_mode( DM_LINE    );
    else if( c == 's' ) set_active_mode( DM_SEGMENT );
    else if( c == 'D' ) set_active_mode( DM_NONE    );
    else if( c == 'C' ) m_gui_circles.clear();
    else if( c == 'S' ) m_gui_segments.clear();
    else if( c == 'P' ) m_gui_pnts.clear();
    else if( c == 'L' ) m_gui_lines.clear();
    else if( c == 'R' ) {
        save_points();
        save_lines();
    }

}

void GUI::catch_mouse() {
    if( wmain.mouse_move_event(gx,gy) ) {
        draw_mouse_shadow();
    }
    if( !wmain.mouse_click(1, gx, gy) )
        return;
    if( m_verbose )
        printf( "clicked coordinate : [xy %d %d]\n", gx, gy );

    switch( m_dmode ) {
    case DM_NONE: break;
    case DM_POINT:
        m_gui_pnts.push_back( Point2d(gx,gy) );
        break;
    case DM_LINE:
        if( l0 ) {
            lx0 = gx;
            ly0 = gy;
            l0 = false;
        } else {
            double r = sqrt( sq(gx-lx0) + sq(gy-ly0) );
            if( r < 1 ) {
                printf("canceled - line has 0 size\n");
            } else {
                Line2d line(lx0, ly0, gx, gy);
                m_gui_lines.push_back( line );
                if( m_verbose ) printf("new line added [ %f %f %f ]\n", line.a, line.b, line.c );
            }
                l0 = true;
        }
        break;
    case DM_SEGMENT:
        if( l0 ) {
            lx0 = gx;
            ly0 = gy;
            l0 = false;
        } else {
            double r = sqrt( sq(gx-lx0) + sq(gy-ly0) );
            if( r < 1 ) {
                printf("canceled - segment has 0 size\n");
            } else {
                Segment2d segment(lx0, ly0, gx, gy);
                m_gui_segments.push_back( segment );
                if( m_verbose ) printf("new segment added [ %f %f %f %f ]\n", segment.x0, segment.y0, segment.x1, segment.y1 );
            }
            l0 = true;
        }
        break;

    case DM_CIRCLE:
        if( l0 ) {
            lx0 = gx;
            ly0 = gy;
            l0  = false;
        } else {
            double r = sqrt( sq(gx-lx0) + sq(gy-ly0) );
            if( r < 1 ) {
                printf("canceled - circle has 0 radius\n");
            } else {
                Circle2d circ( lx0, ly0, r );
                m_gui_circles.push_back( circ );
                if( m_verbose )
                    printf( "[center: %f %f] [rad %f]\n", circ.cx, circ.cy, circ.r );
            }
            l0 = true;
        }
        break;
    }

    wmain.reset_mouse();
}

void GUI::display() {
    wmain.reset_mouse();
    toggle_zoom_window();
    while( 1 ) {
        reset_display();
        catch_keyboard();
        catch_mouse();
        draw_points();
        draw_lines();
        draw_circles();
        update_zoom_window();
        display_help();
        refresh();
    }
}

void GUI::reset_display() {
    wmain.reset_display();
}

void GUI::update_zoom_window() {
    if( !wzoom ) return;
    wzoom->set_display( &wmain, gx, gy, m_zsz );
    int rw = m_zsz/2.5;
    int px = m_zsz/2 - rw;
    int py = m_zsz/2 - rw;
    wzoom->draw_rectangle( px, py, 2*rw+1, 2*rw+1 );
}

void GUI::refresh() {
    wmain.refresh();
    if( wzoom ) wzoom->refresh();
}

void GUI::draw_mouse_shadow() {
    if( !m_b_enable_shadow ) return;

    wmain.set_color( 255, 255, 255 );
    wmain.mark( gx, gy, 0 );

    wmain.set_color( 0, 255, 0 );
    wmain.draw_line( gx, gy+4, gx, gy+17 );
    wmain.draw_line( gx, gy-4, gx, gy-17 );
    wmain.draw_line( gx+4, gy, gx+17, gy );
    wmain.draw_line( gx-4, gy, gx-17, gy );

    wmain.set_color( 255, 0, 0 );
    wmain.draw_line( gx+3, gy-3, gx+15, gy-15 );
    wmain.draw_line( gx+3, gy+3, gx+15, gy+15 );
    wmain.draw_line( gx-3, gy-3, gx-15, gy-15 );
    wmain.draw_line( gx-3, gy+3, gx-15, gy+15 );

    wmain.draw_circle( gx, gy, 15 );
}

void GUI::draw_points() {
    if( m_pnts ) {
        wmain.set_color(255,0,255);
        for( int p=0; p<(int)m_pnts->size(); p++ ) {
            const Point2d& pnt = m_pnts->at(p);
            wmain.mark( pnt.x, pnt.y );
        }
    }

    if( m_gui_pnts.size() ) {
        wmain.set_color(0, 255, 0);
        for( int p=0; p<(int)m_gui_pnts.size(); p++ ) {
            const Point2d& pnt = m_gui_pnts[p];
            wmain.mark( pnt.x, pnt.y );
        }
    }
}

void GUI::draw_circles() {
    wmain.set_color(255,255,255);

    for( int i=0; i<(int)m_gui_circles.size(); i++ ) {
        const Circle2d& circ = m_gui_circles[i];
        wmain.draw_circle( circ.cx, circ.cy, circ.r );
    }
    if( m_dmode == DM_CIRCLE && !l0 ) {
        wmain.mark( lx0, ly0, 1 );
        wmain.draw_line( lx0, ly0, gx, gy );
        double r = sqrt( sq(gx-lx0) + sq(gy-ly0) );
        wmain.draw_circle( lx0, ly0, r );
        wmain.write( lx0, ly0, num2str(r) );
        wmain.write( gx, gy, "("+num2str(gx)+","+num2str(gy)+")" );
    }
}

void GUI::draw_lines() {
    if( m_lines ) {
        uchar cr, cg, cb;
        for( int l=0; l<(int)m_lines->size(); l++ ) {
            const Line2d& line = m_lines->at(l);
            get_color( l+1, cr, cg, cb );
            wmain.set_color( cr, cg, cb );
            if( !line.is_vertical() )
                wmain.draw_line( 0, line.y_intercept(0), gw, line.y_intercept(gw) );
            else
                wmain.draw_line( line.x_intercept(0), 0, line.x_intercept(gh), gh );
        }
    }

    if( m_gui_lines.size() ) {
        wmain.set_color( 255, 255, 255 );
        for( int l=0; l<(int)m_gui_lines.size(); l++ ) {
            const Line2d& line = m_gui_lines[l];
            if( !line.is_vertical() )
                wmain.draw_line( 0, line.y_intercept(0), gw, line.y_intercept(gw) );
            else
                wmain.draw_line( line.x_intercept(0), 0, line.x_intercept(gh), gh );
        }
    }

    if( m_gui_segments.size() ) {
        for( int l=0; l<(int)m_gui_segments.size(); l++ ) {
            const Segment2d& seg = m_gui_segments[l];
            wmain.set_color( 255, 255, 0 );
            wmain.draw_line( seg.x0, seg.y0, seg.x1, seg.y1 );
            wmain.set_color( 0, 255, 255 );
            wmain.mark( seg.x0, seg.y0, 1 );
            wmain.mark( seg.x1, seg.y1, 1 );
        }
    }

    if( ( m_dmode==DM_LINE || m_dmode==DM_SEGMENT ) && !l0 ) {
        wmain.set_color(255,255,0);
        wmain.mark( lx0, ly0, 1 );
        wmain.draw_line( lx0, ly0, gx, gy );
        wmain.write( gx, gy, "("+num2str(gx)+","+num2str(gy)+")" );
    }

}

void GUI::display_help() {
    if( !m_b_enable_help ) return;
    wmain.set_color( 255, 255, 10 );
    wmain.write( 10,  20, "h: toggle help text" );
    wmain.write( 10,  40, "v: toggle verbosity" );
    wmain.write( 10,  60, "q: quit" );
    wmain.write( 10,  80, "z: toggle zoom window" );
    wmain.write( 10, 100, "m: enable mouse shadow" );

    wmain.write( 10, 120, "p: draw point   / P clears" );
    wmain.write( 10, 140, "l: draw line    / L clears" );
    wmain.write( 10, 160, "s: draw segment / S clears" );
    wmain.write( 10, 180, "c: draw circle  / C clears" );
    wmain.write( 10, 200, "D: no draw" );
    wmain.write( 10, 220, "R: save points & lines" );
}

void GUI::save_points() const {
    ofstream fout;
    string file = "/tmp/gui_points.txt";
    open_or_fail( file, fout );
    int np = m_gui_pnts.size();
    fout << np << endl;
    for( int i=0; i<np; i++ ) {
        const Point2d& pnt = m_gui_pnts[i];
        fout << pnt.x << " " << pnt.y << endl;
    }
    fout.close();
    printf("save gui points in [%s]\n", file.c_str() );
}

void GUI::save_lines() const {
    ofstream fout;
    string file = "/tmp/gui_lines.txt";
    open_or_fail( file, fout );
    int np = m_gui_lines.size();
    fout << np << endl;
    for( int i=0; i<np; i++ ) {
        const Line2d& line = m_gui_lines[i];
        fout << line.a << " " << line.b << " " << line.c << endl;
    }
    fout.close();
    printf("save gui lines in [%s]\n", file.c_str() );
}
