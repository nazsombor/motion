//
// Created by abraham on 8/12/24.
//

#ifndef CAIROMOTION_H
#define CAIROMOTION_H

#include <gtkmm.h>
#include <iostream>

#include "Drawings.h"
#include "ui/Placeholder.h"
#include "ui/PopupBar.h"
#include "ui/Tools.h"
#include "ui/Canvas.h"
#include "ui/CanvasContainer.h"
#include "ui/ScrolledWidget.h"

class Cairomotion : public Gtk::Window {
    Placeholder p1, p2;
    Tools tools;
    ScrolledWidget sw1, sw2;
    CanvasContainer c1;
    Canvas canvas;
    PopupBar pb2, pb1;
    Glib::RefPtr<Gtk::GestureClick> gc;
    Glib::RefPtr<Gtk::GestureStylus> gs;
    Glib::RefPtr<Gtk::EventControllerKey> eck;

    Drawings drawings;

    // canvas resize variables
    bool start_window_size_change = false;
    bool allow_canvas_resize_once_per_window_resize = false;
    int window_width, window_height;
    int canvas_width = 1920, canvas_height = 1080;
    gint64 window_size_change_timer = 0;
    bool popup_visibility_changed;
    gint64 popup_visibility_change_timer = 0;
    bool allow_canvas_resize_just_after_toogling_popups;

    bool is_window_fullscreen = false;


    void on_click(int type, double x, double y);

public:
    Cairomotion();

protected:
    void size_allocate_vfunc(int width, int height, int baseline) override;

    void on_key_released(guint key, guint _, Gdk::ModifierType m_type);

    bool tick(const Glib::RefPtr<Gdk::FrameClock> &clock);
};


#endif //CAIROMOTION_H
