//
// Created by abraham on 8/17/24.
//

#ifndef SCROLLEDWIDGET_H
#define SCROLLEDWIDGET_H

#include <gtkmm.h>
#include <iostream>

class Viewport : public Gtk::Viewport {
    int width = 100;
    int height = 100;

public:

    Viewport(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v);

    Gtk::SizeRequestMode get_request_mode_vfunc() const override;

    void measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural, int &minimum_baseline, int &natural_baseline) const override;

    void resize(int width, int height);
};



class ScrolledWidget : public Gtk::Box{
    Gtk::Box b1, b2;
    Glib::RefPtr<Gtk::Adjustment> h, v;
    Viewport vp;
    Gtk::Scrollbar hs, vs;

public:
    ScrolledWidget();

    void set_child(Gtk::Widget &widget);

    void resize(int width, int height);
};



#endif //SCROLLEDWIDGET_H
