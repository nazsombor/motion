//
// Created by abraham on 8/12/24.
//

#ifndef POPUPBAR_H
#define POPUPBAR_H
#include <gtkmm.h>

#include "ui/Canvas.h"


class PopupBar : public Gtk::Box {
public:
    enum Align {LEFT, BOTTOM};
private:
    Align alignment;
    Gtk::Widget *space;
    Gtk::Widget *popup;
    Canvas *canvas;
    Glib::RefPtr<Gtk::EventControllerMotion> ecm;
    bool is_popup_visible = true;

public:
    PopupBar(Gtk::Widget *space, Gtk::Widget *popup, Canvas *canvas, Align align);

    void mouse_enter(double x, double y);

    void mouse_leave();

    void toogle_bar_visibility();
};



#endif //POPUPBAR_H
