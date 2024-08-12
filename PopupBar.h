//
// Created by abraham on 8/12/24.
//

#ifndef POPUPBAR_H
#define POPUPBAR_H
#include <gtkmm/box.h>


class PopupBar : public Gtk::Box {
public:
    PopupBar(Gtk::Widget * spaceWidget, Gtk::Widget *popupWidget);
};



#endif //POPUPBAR_H
