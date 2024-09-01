//
// Created by abraham on 8/31/24.
//

#ifndef COLORPICKER_H
#define COLORPICKER_H
#include <gtkmm.h>


class ColorPicker : public Gtk::Box{
    Gtk::DrawingArea drawing_area;
    Gtk::Scrollbar scrollbar;
    const Glib::RefPtr<Gtk::Adjustment> adjustment;
    int value = 0;
    Glib::RefPtr<Gtk::GestureClick> click;
    double r, g, b;

public:
    ColorPicker();

    void on_draw(const Glib::RefPtr<Cairo::Context> &cr, int width, int height);

    void on_adjustment_changed();

    void on_click(int count, double x, double y);
};



#endif //COLORPICKER_H
