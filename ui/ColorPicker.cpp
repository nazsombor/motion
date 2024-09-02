//
// Created by abraham on 8/31/24.
//

#include "ColorPicker.h"

#include <iostream>
#include <vector>


ColorPicker::ColorPicker() : adjustment(Gtk::Adjustment::create(0, 0, 255)){
    set_orientation(Gtk::Orientation::VERTICAL);
    set_halign(Gtk::Align::START);
    append(drawing_area);
    append(scrollbar);
    drawing_area.set_content_width(256);
    drawing_area.set_content_height(256);
    drawing_area.set_draw_func(sigc::mem_fun(*this, &ColorPicker::on_draw));
    drawing_area.set_halign(Gtk::Align::START);
    scrollbar.set_orientation(Gtk::Orientation::HORIZONTAL);
    scrollbar.set_adjustment(adjustment);
    adjustment->signal_value_changed().connect(sigc::mem_fun(*this, &ColorPicker::on_adjustment_changed));
    click = Gtk::GestureClick::create();
    click->signal_pressed().connect(sigc::mem_fun(*this, &ColorPicker::on_click));
    drawing_area.add_controller(click);
}

void ColorPicker::on_draw(const Glib::RefPtr<Cairo::Context> &cr, int width, int height) {
    std::cout << "It draws" << std::endl;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double lr = (double) x / width, lg = (double) y / height, lb = (double) adjustment->get_value() / 256.0;
            cr->set_source_rgb(lr, lg, lb);
            cr->rectangle(x, y, 1, 1);
            cr->fill();
        }
    }
    double lr = (double) x / width, lg = (double) y / height, lb = (double) adjustment->get_value() / 256.0;
    cr->set_source_rgb(lr, lg, lb);
    cr->rectangle(x - 10, y - 10, 20, 20);
    cr->fill_preserve();
    cr->set_source_rgb(0, 0, 0);
    cr->stroke();
}

void ColorPicker::on_adjustment_changed() {
    drawing_area.queue_draw();
}

void ColorPicker::on_click(int count, double x, double y) {
    std::cout << "Clicked at (" << x << ", " << y << ")" << std::endl;
    r = x / get_width(), g = y / get_height(), b = adjustment->get_value() / 256.0;
    this->x = x; this->y = y;
    drawing_area.queue_draw();
}
