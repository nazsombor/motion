//
// Created by abraham on 8/12/24.
//

#ifndef CAIROMOTION_H
#define CAIROMOTION_H

#include <gtkmm.h>
#include <iostream>

#include "Placeholder.h"
#include "PopupBar.h"

class Cairomotion : public Gtk::Window {
    Placeholder p1;
    Placeholder p2;
    Placeholder p3;
    Gtk::CenterBox c1;
    PopupBar pb2;
    PopupBar pb1;
    Glib::RefPtr<Gtk::GestureClick> gc;
    bool canvas_is_maximized = false;

    void on_click(int type, double x, double y);

public:
    Cairomotion();



};


#endif //CAIROMOTION_H
