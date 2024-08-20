//
// Created by abraham on 8/12/24.
//

#include "PopupBar.h"

#include "Placeholder.h"

PopupBar::PopupBar(Gtk::Widget *spaceWidget, Gtk::Widget *popupWidget, Canvas *canvas, Align align) : p(30, 30, Placeholder::Color::BLUE){
    alignment = align;
    space = spaceWidget;
    popup = popupWidget;
    this->canvas = canvas;
    sw.set_child(*popup);
    sw.set_visible(false);
    switch (alignment) {
        case BOTTOM:
            space->set_vexpand(true);
            set_orientation(Gtk::Orientation::VERTICAL);
            b.set_orientation(Gtk::Orientation::VERTICAL);
            b.append(sw);
            b.append(p);
            append(*space);
            append(b);
            break;
        case LEFT:
            space->set_hexpand(true);
            set_orientation(Gtk::Orientation::HORIZONTAL);
            b.set_orientation(Gtk::Orientation::HORIZONTAL);
            b.append(p);
            b.append(sw);
            append(b);
            append(*space);
            break;
    }
    ecm = Gtk::EventControllerMotion::create();
    ecm->signal_enter().connect(sigc::mem_fun(*this, &PopupBar::mouse_enter));
    ecm->signal_leave().connect(sigc::mem_fun(*this, &PopupBar::mouse_leave));

    b.add_controller(ecm);
}

void PopupBar::mouse_enter(double x, double y) {
    switch (alignment) {
        case BOTTOM: {
            canvas->resize(get_width(), get_height() - std::min(space->get_height() + 100, 800));
            sw.set_visible();
            sw.resize(get_width() - 20,std::min(space->get_height(), 700));
            p.set_visible(false);
            break;
        }
        case LEFT: {
            canvas->resize(get_width() - std::min(space->get_width() + 100, 800), get_height());
            sw.set_visible();
            sw.resize(std::min(space->get_width(), 700), get_height() - 20);
            p.set_visible(false);
            break;
        }
    }
}

void PopupBar::mouse_leave() {
    switch (alignment) {
        case BOTTOM: {
            canvas->resize(get_width() - 30, get_height() - 30);
            sw.set_visible(false);
            p.set_visible();
            break;
        }
        case LEFT: {
            canvas->resize(get_width() - 30, get_height() - 30);
            sw.set_visible(false);
            p.set_visible();
            break;
        }
    }
}

void PopupBar::toogle_bar_visibility() {
    if (is_popup_visible) {
        is_popup_visible = false;
        switch (alignment) {
            case BOTTOM: {
                p.set_content_height(0);
                sw.set_visible(false);
                canvas->resize(get_width(), get_height());
                break;
            }
            case LEFT: {
                p.set_content_width(0);
                sw.set_visible(false);
                canvas->resize(get_width(), get_height());
                break;
            }
        }
    } else {
        is_popup_visible = true;
        switch (alignment) {
            case BOTTOM: {
                p.set_content_height(30);
                sw.set_visible(false);
                canvas->resize(get_width(), get_height() - 30);
                break;
            }
            case LEFT: {
                p.set_content_width(30);
                sw.set_visible(false);
                canvas->resize(get_width() - 30, get_height());
                break;
            }
        }
    }
}
