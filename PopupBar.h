//
// Created by abraham on 8/12/24.
//

#ifndef POPUPBAR_H
#define POPUPBAR_H
#include <gtkmm/box.h>


class PopupBar : public Gtk::Box {
public:
    enum Align {LEFT, BOTTOM};
private:
    Align alignment;
    Gtk::Widget *space;
    Gtk::Widget *popup;

public:
    PopupBar(Gtk::Widget * space, Gtk::Widget *popup, Align align);
};



#endif //POPUPBAR_H
