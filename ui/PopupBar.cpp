//
// Created by abraham on 8/12/24.
//

#include "../PopupBar.h"

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
            canvas->resize(canvas->get_width(), get_height() - std::min(space->get_height(), 700));
            ((Gtk::DrawingArea*) popup)->set_content_height(std::min(get_height() - 30, 700));
            break;
        }
        case LEFT: {
            canvas->resize(get_width() - std::min(space->get_width(), 700), canvas->get_height());
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

void PopupBar::toogle_bar_visibility() {
    if (is_popup_visible) {
        is_popup_visible = false;
        switch (alignment) {
            case BOTTOM: {
                ((Gtk::DrawingArea*) popup)->set_content_height(0);
                canvas->resize(get_width(), get_height());
                break;
            }
            case LEFT: {
                ((Gtk::DrawingArea*) popup)->set_content_width(0);
                canvas->resize(get_width(), get_height());
                break;
            }
        }
    } else {
        is_popup_visible = true;
        switch (alignment) {
            case BOTTOM: {
                ((Gtk::DrawingArea*) popup)->set_content_height(30);
                canvas->resize(get_width(), get_height() - 30);
                break;
            }
            case LEFT: {
                ((Gtk::DrawingArea*) popup)->set_content_width(30);
                canvas->resize(get_width() - 30, get_height());
                break;
            }
        }
    }
}
