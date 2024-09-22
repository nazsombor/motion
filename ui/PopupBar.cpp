//
// Created by abraham on 8/12/24.
//

#include "PopupBar.h"

#include "Placeholder.h"

PopupBar::PopupBar(Gtk::Widget *spaceWidget, Gtk::Widget *popupWidget, Canvas *canvas, Alignment align) : bar(30, 30, Placeholder::Color::BLUE){
    alignment = align;
    space = spaceWidget;
    popup = popupWidget;
    this->canvas = canvas;
    popup->set_visible(false);
    switch (alignment) {
        case BOTTOM:
            space->set_vexpand(true);
            set_orientation(Gtk::Orientation::VERTICAL);
            container.set_orientation(Gtk::Orientation::VERTICAL);
            container.append(*popup);
            container.append(bar);
            append(*space);
            append(container);
            break;
        case LEFT:
            space->set_hexpand(true);
            set_orientation(Gtk::Orientation::HORIZONTAL);
            container.set_orientation(Gtk::Orientation::HORIZONTAL);
            container.append(bar);
            container.append(*popup);
            append(container);
            append(*space);
            break;
    }
    ecm = Gtk::EventControllerMotion::create();
    ecm->signal_enter().connect(sigc::mem_fun(*this, &PopupBar::mouse_enter));
    ecm->signal_leave().connect(sigc::mem_fun(*this, &PopupBar::mouse_leave));

    container.add_controller(ecm);
}

void PopupBar::mouse_enter(double x, double y) {
    switch (alignment) {
        case BOTTOM: {
            canvas->resize(get_width(), get_height() - std::min(space->get_height() + 100, 800));
            popup->set_visible(true);
            bar.set_visible(false);
            break;
        }
        case LEFT: {
            canvas->resize(get_width() - std::min(space->get_width() + 100, 800), get_height());
            popup->set_visible(true);
            bar.set_visible(false);
            break;
        }
    }
}

void PopupBar::mouse_leave() {
    switch (alignment) {
        case BOTTOM: {
            canvas->resize(get_width() - 30, get_height() - 30);
            popup->set_visible(false);
            bar.set_visible();
            break;
        }
        case LEFT: {
            canvas->resize(get_width() - 30, get_height() - 30);
            popup->set_visible(false);
            bar.set_visible();
            break;
        }
    }
}

void PopupBar::toogle_bar_visibility() {
    if (is_popup_visible) {
        is_popup_visible = false;
        switch (alignment) {
            case BOTTOM: {
                bar.set_content_height(0);
                canvas->resize(get_width(), get_height());
                break;
            }
            case LEFT: {
                bar.set_content_width(0);
                canvas->resize(get_width(), get_height());
                break;
            }
        }
    } else {
        is_popup_visible = true;
        switch (alignment) {
            case BOTTOM: {
                bar.set_content_height(30);
                canvas->resize(get_width(), get_height() - 30);
                break;
            }
            case LEFT: {
                bar.set_content_width(30);
                canvas->resize(get_width() - 30, get_height());
                break;
            }
        }
    }
}
