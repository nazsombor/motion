//
// Created by abraham on 9/9/24.
//

#include "CurrentColor.h"

CurrentColor::CurrentColor(ColorList *list) {
    this->list = list;

    set_orientation(Gtk::Orientation::HORIZONTAL);

    color.set_content_width(50);
    color.set_content_height(50);
    color.set_draw_func(sigc::mem_fun(*this, &CurrentColor::on_draw));
    append(color);

    append(text);

    add_button.signal_clicked().connect(sigc::mem_fun(*this, &CurrentColor::on_add_button_clicked));
    add_button.set_label("Add");
    append(add_button);

    update_button.signal_clicked().connect(sigc::mem_fun(*this, &CurrentColor::on_update_button_clicked));
    update_button.set_label("Update");
    append(update_button);

    pick_button.signal_clicked().connect(sigc::mem_fun(*this, &CurrentColor::on_pick_button_clicked));
    pick_button.set_label("Pick");
    append(pick_button);

    display = XOpenDisplay(0);
    window = RootWindow(display, 0);
    XGetWindowAttributes(display, window, &attributes);

    XSelectInput(display, window, ButtonPressMask | ButtonReleaseMask);
    XFlush(display);
}

void CurrentColor::update_color(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
    color.queue_draw();
}

void CurrentColor::on_add_button_clicked() {
    list->add_color(r, g, b, text.get_text());
}

void CurrentColor::on_pick_button_clicked() {
    pick_button_clicked = true;
}

void CurrentColor::on_update_button_clicked() {
}

void CurrentColor::on_draw(const Cairo::RefPtr<Cairo::Context> &context, int width, int height) {
    context->set_source_rgb(r / 255.0, g / 255.0, b / 255.0);
    context->rectangle(0, 0, width, height);
    context->fill();
}
