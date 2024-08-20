//
// Created by abraham on 8/17/24.
//

#include "ScrolledWidget.h"

Viewport::Viewport(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v) : Gtk::Viewport(h, v){
}

Gtk::SizeRequestMode Viewport::get_request_mode_vfunc() const {
    return Gtk::SizeRequestMode::CONSTANT_SIZE;
}

void Viewport::measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural,
    int &minimum_baseline, int &natural_baseline) const {

    minimum_baseline = natural_baseline = -1;

    if (orientation == Gtk::Orientation::HORIZONTAL) {
        minimum = width;
        natural = width;
    } else {
        minimum = height;
        natural = height;
    }

}

void Viewport::resize(int width, int height) {
    this->width = width;
    this->height = height;
}

ScrolledWidget::ScrolledWidget() : h(Gtk::Adjustment::create(0, 0, 1000)), v(Gtk::Adjustment::create(0, 0, 1000)), vp(h, v){
    hs.set_adjustment(vp.get_hadjustment());
    vs.set_adjustment(vp.get_vadjustment());
    vs.set_orientation(Gtk::Orientation::VERTICAL);
    b1.set_orientation(Gtk::Orientation::HORIZONTAL);
    b2.set_orientation(Gtk::Orientation::VERTICAL);
    b1.append(b2);
    b1.append(vs);
    b2.append(vp);
    b2.append(hs);
    append(b1);
}

void ScrolledWidget::set_child(Gtk::Widget &widget) {
    vp.set_child(widget);
}

void ScrolledWidget::resize(int width, int height) {
    vp.resize(width, height);
}
