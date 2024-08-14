//
// Created by abraham on 8/12/24.
//

#include "Cairomotion.h"

void Cairomotion::on_click(int type, double x, double y) {
    auto button = gc->get_current_event()->get_button();
    switch(button) {
        case GDK_BUTTON_MIDDLE: {
            std::cout << "Middle stylus button" << std::endl;
            break;
        }
        case GDK_BUTTON_PRIMARY: {
            std::cout << "Primary stylus button" << std::endl;
            break;
        }
        case GDK_BUTTON_SECONDARY: {
            std::cout << "Secondary stylus button" << std::endl;
            break;
        }
    }
}

Cairomotion::Cairomotion(): p1(30, 30, Placeholder::RED),
                            p3(30, 30, Placeholder::YELLOW),
                            pb2(&c1, &p3, &canvas, PopupBar::LEFT),
                            pb1(&pb2, &p1, &canvas, PopupBar::BOTTOM) {
    canvas.set_valign(Gtk::Align::CENTER);
    canvas.set_content_width(600);
    canvas.set_content_height(300);
    c1.set_center_widget(canvas);

    auto style = Gtk::CssProvider::create();
    style->load_from_data(".center-container{ background-color: #ccc; }");
    Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), style, 0);
    c1.add_css_class("center-container");

    set_default_size(1300, 900);
    set_child(pb1);

    gc = Gtk::GestureClick::create();
    gc->signal_released().connect(sigc::mem_fun(*this, &Cairomotion::on_click));
    gc->set_button(0);
    add_controller(gc);

    add_tick_callback(sigc::mem_fun(*this, &Cairomotion::tick));
}

void Cairomotion::size_allocate_vfunc(int width, int height, int baseline) {
    if (window_width != width || window_height != height) {
        canvas.set_content_width(0);
        canvas.set_content_height(0);
        start_window_size_change = true;
    }
    window_width = width;
    window_height = height;
    Gtk::Widget::size_allocate_vfunc(width, height, baseline);
}

bool Cairomotion::tick(const Glib::RefPtr<Gdk::FrameClock> &clock) {
    if (start_window_size_change) {
        window_size_change_timer = clock->get_frame_time();
        start_window_size_change = false;
        allow_canvas_resize_once_per_window_resize = true;
    }
    if ((clock->get_frame_time() - window_size_change_timer) > 250000 &&
        allow_canvas_resize_once_per_window_resize) {
        canvas.resize(c1.get_width(), c1.get_height());
        allow_canvas_resize_once_per_window_resize = false;
        }
    return true;
}