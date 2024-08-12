//
// Created by abraham on 8/12/24.
//

#include "PopupBar.h"

PopupBar::PopupBar(Gtk::Widget *spaceWidget, Gtk::Widget *popupWidget, Align align) {
    alignment = align;
    space = spaceWidget;
    popup = popupWidget;
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
}
