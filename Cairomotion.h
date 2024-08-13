//
// Created by abraham on 8/12/24.
//

#ifndef CAIROMOTION_H
#define CAIROMOTION_H

#include <gtkmm.h>
#include <iostream>

#include "Placeholder.h"
#include "PopupBar.h"
#include "ui/CanvasContainer.h"

class Cairomotion : public Gtk::Window {
    Placeholder p1;
    Placeholder p2;
    Placeholder p3;
    CanvasContainer c1;
    PopupBar pb2;
    PopupBar pb1;
    Glib::RefPtr<Gtk::GestureClick> gc;
    bool canvas_is_maximized = false;
    bool start_window_size_change = false;
    bool allow_canvas_resize_once_per_window_resize = false;
    int window_width, window_height;
    int canvas_width = 1920, canvas_height = 1080;
    gint64 window_size_change_timer = 0;

    void on_click(int type, double x, double y);

public:
    Cairomotion();

protected:
    void size_allocate_vfunc(int width, int height, int baseline) override {
        if (window_width != width || window_height != height) {
            p2.set_content_width(0);
            p2.set_content_height(0);
            start_window_size_change = true;
        }
        window_width = width;
        window_height = height;
        Gtk::Widget::size_allocate_vfunc(width, height, baseline);
    }

    bool tick(const Glib::RefPtr<Gdk::FrameClock> &clock) {
        if (start_window_size_change) {
            window_size_change_timer = clock->get_frame_time();
            start_window_size_change = false;
            allow_canvas_resize_once_per_window_resize = true;
        }
        if ((clock->get_frame_time() - window_size_change_timer) > 250000 &&
            allow_canvas_resize_once_per_window_resize) {
            auto cR = (float) c1.get_width() / canvas_width;

            auto w = canvas_width * cR;
            auto h = canvas_height * cR;

            if (h > c1.get_height()) {
                cR = (float) c1.get_height() / canvas_height;
                w = canvas_width * cR;
                h = canvas_height * cR;
            }

            p2.set_content_width(w);
            p2.set_content_height(h);
            allow_canvas_resize_once_per_window_resize = false;
        }
        return true;
    }
};


#endif //CAIROMOTION_H
