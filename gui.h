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
#ifndef KORTEX_MLF_GUI_H
#define KORTEX_MLF_GUI_H

#include <vector>
using std::vector;

#include <kortex/gui_window.h>
using namespace kortex;

#include "structs.h"

enum DrawMode { DM_NONE, DM_POINT, DM_LINE, DM_SEGMENT, DM_CIRCLE };

class GUI {
public:
    GUI();

    void setup( const vector<Point2d>* pnts, const vector<Line2d>* lines );
    void create( int w, int h );
    void display();

private:
    GUIWindow  wmain;
    GUIWindow* wzoom;
    int gx, gy, gw, gh;
    int m_zsz;
    bool m_b_enable_shadow;
    bool m_b_enable_help;
    bool m_verbose;
    DrawMode m_dmode;

    void refresh();
    void reset_display();
    void reset_mouse();
    void catch_keyboard();
    void catch_mouse();

    void draw_mouse_shadow();
    void draw_points();
    void draw_lines();
    void draw_circles();

    void display_help();

    void set_active_mode( DrawMode mode );

    void toggle_zoom_window();
    void update_zoom_window();

    void save_points() const;
    void save_lines() const;

    bool l0;
    int lx0, ly0;

    vector<Segment2d> m_gui_segments;
    vector<Point2d>   m_gui_pnts;
    vector<Line2d >   m_gui_lines;
    vector<Circle2d>  m_gui_circles;

    const vector<Point2d>* m_pnts;
    const vector<Line2d> * m_lines;
};



#endif

