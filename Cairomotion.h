//
// Created by abraham on 8/12/24.
//

#ifndef CAIROMOTION_H
#define CAIROMOTION_H

#include <gtkmm.h>
#include <iostream>

#include "Drawings.h"
#include "ui/PopupBar.h"
#include "ui/Tools.h"
#include "ui/Canvas.h"
#include "ui/CanvasContainer.h"
#include "ui/Timeline.h"

inline bool util_widget_is_focused (Gtk::Widget& widget)
{
    const Gtk::Root *root = widget.get_root ();
    if (!root)
        return false;

    const Gtk::Widget *focused = root->get_focus ();
    if (!focused)
        return false;

    return focused->is_ancestor (widget);
}

class Cairomotion : public Gtk::Window {
    Canvas canvas;
    Tools tools;
    Timeline timeline;
    CanvasContainer container;
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
    bool allow_canvas_resize_just_after_toggling_popups = false;

    bool is_window_fullscreen = false;

public:
    Cairomotion();

private:
    void on_click(int type, double x, double y);

protected:
    void size_allocate_vfunc(int width, int height, int baseline) override;

    void on_key_released(guint key, guint _, Gdk::ModifierType m_type);

    void handle_window_resize(const Glib::RefPtr<Gdk::FrameClock> &clock);

    void handle_play(const Glib::RefPtr<Gdk::FrameClock> &clock);

    void handle_update_color_picker();

    void handle_pick_color_from_anywhere_the_screen();

    void handle_save();

    void handle_open();

    bool tick(const Glib::RefPtr<Gdk::FrameClock> &clock);
};

#define Status int

#include <X11/Xutil.h>


#endif //CAIROMOTION_H
