//
// Created by abraham on 8/31/24.
//

#include "ColorPicker.h"

#include <iostream>
#include <vector>


ColorPicker::ColorPicker(CurrentColor *current_color) : adjustment(Gtk::Adjustment::create(0, 0, 255)){
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
    this->current_color = current_color;
    current_color->r = r;
    current_color->g = g;
    current_color->b = b;

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
    current_color->update_color(x, y, adjustment->get_value());
    b = adjustment->get_value() / 256.0;
}

void ColorPicker::on_click(int count, double x, double y) {
    r = x / get_width(), g = y / get_height(), b = adjustment->get_value() / 256.0;
    this->x = x; this->y = y;
    current_color->update_color(x, y, adjustment->get_value());
    drawing_area.queue_draw();
}

void ColorPicker::update(int r, int g, int b) {
    this->r = r / 255.0, this->g = g / 255.0, this->b = b / 255.0;
    this->x = r;
    this->y = g;
    adjustment->set_value(b);
}
