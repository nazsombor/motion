//
// Created by abraham on 8/12/24.
//

#ifndef POPUPBAR_H
#define POPUPBAR_H
#include <gtkmm.h>

#include "Canvas.h"
#include "Placeholder.h"
#include "ScrolledWidget.h"


class PopupBar : public Gtk::Box {
public:
    enum Alignment {LEFT, BOTTOM};
private:
    Alignment alignment;
    Gtk::Box container;
    Placeholder p;
    ScrolledWidget sw;
    Gtk::Widget *space;
    Gtk::Widget *popup;
    Canvas *canvas;
    Glib::RefPtr<Gtk::EventControllerMotion> ecm;
    bool is_popup_visible = true;

public:
    PopupBar(Gtk::Widget *space, Gtk::Widget *popup, Canvas *canvas, Alignment align);

    void mouse_enter(double x, double y);

    void mouse_leave();

    void toogle_bar_visibility();
};



#endif //POPUPBAR_H
