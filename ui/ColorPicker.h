//
// Created by abraham on 8/31/24.
//

#ifndef COLORPICKER_H
#define COLORPICKER_H
#include <gtkmm.h>

#include "ColorList.h"
#include "CurrentColor.h"


class ColorPicker : public Gtk::Box{
    Gtk::Scrollbar scrollbar;
    int value = 0;
    Glib::RefPtr<Gtk::GestureClick> click;
    CurrentColor * current_color;

public:
    Gtk::DrawingArea drawing_area;
    double r = 0.0, g = 0.0, b = 0.0;
    int x = 0, y = 0;
    const Glib::RefPtr<Gtk::Adjustment> adjustment;

    ColorPicker(CurrentColor *current_color);

    void on_draw(const Glib::RefPtr<Cairo::Context> &cr, int width, int height);

    void on_adjustment_changed();

    void on_click(int count, double x, double y);
};



#endif //COLORPICKER_H
