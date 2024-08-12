//
// Created by abraham on 8/12/24.
//

#include "PopupBar.h"

PopupBar::PopupBar(Gtk::Widget *spaceWidget, Gtk::Widget *popupWidget) {
    spaceWidget->set_vexpand(true);
    set_orientation(Gtk::Orientation::VERTICAL);
    append(*spaceWidget);
    append(*popupWidget);
}
