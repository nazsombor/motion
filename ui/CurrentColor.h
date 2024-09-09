//
// Created by abraham on 9/9/24.
//

#ifndef CURRENTCOLOR_H
#define CURRENTCOLOR_H
#include <gtkmm.h>

#include "ColorList.h"


class CurrentColor : public Gtk::Box{
public:
    Gtk::DrawingArea color;
    Gtk::Entry text;
    Gtk::Button add_button;
    ColorList * list;
    int r, g, b;

    CurrentColor(ColorList *list);

    void update_color(int r, int g, int b);

    void on_add_button_clicked();

    void on_draw(const Cairo::RefPtr<Cairo::Context> &context, int width, int height);

};



#endif //CURRENTCOLOR_H
