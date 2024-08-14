//
// Created by abraham on 8/12/24.
//

#include "PopupBar.h"

PopupBar::PopupBar(Gtk::Widget *spaceWidget, Gtk::Widget *popupWidget, Canvas *canvas, Align align) {
    alignment = align;
    space = spaceWidget;
    popup = popupWidget;
    this->canvas = canvas;
    switch (alignment) {
        case BOTTOM:
            space->set_vexpand(true);
            set_orientation(Gtk::Orientation::VERTICAL);
            append(*space);
            append(*popup);
            break;
        case LEFT:
            space->set_hexpand(true);
            set_orientation(Gtk::Orientation::HORIZONTAL);
            append(*popup);
            append(*space);
            break;
    }
    ecm = Gtk::EventControllerMotion::create();
    ecm->signal_enter().connect(sigc::mem_fun(*this, &PopupBar::mouse_enter));
    ecm->signal_leave().connect(sigc::mem_fun(*this, &PopupBar::mouse_leave));

    popup->add_controller(ecm);
}

void PopupBar::mouse_enter(double x, double y) {
    switch (alignment) {
        case BOTTOM: {
            canvas->resize(canvas->get_width(), get_height() - std::min(get_height(), 700));
            ((Gtk::DrawingArea*) popup)->set_content_height(std::min(get_height(), 700));
            break;
        }
        case LEFT: {
            canvas->resize(get_width() - std::min(get_height(), 700), canvas->get_height());
            ((Gtk::DrawingArea*) popup)->set_content_width(std::min(get_width(), 700));
            break;
        }
    }
}

void PopupBar::mouse_leave() {
    switch (alignment) {
        case BOTTOM: {
            canvas->resize(get_width(), get_height() - 30);
            ((Gtk::DrawingArea*) popup)->set_content_height(30);
            break;
        }
        case LEFT: {
            canvas->resize(get_width() - 30, get_height());
            ((Gtk::DrawingArea*) popup)->set_content_width(30);
            break;
        }
    }
}
