//
// Created by abraham on 8/12/24.
//

#include "Cairomotion.h"

#include "ui/Placeholder.h"
#include "ui/PopupBar.h"

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
            pb1.toogle_bar_visibility();
            pb2.toogle_bar_visibility();
            popup_visibility_changed = true;
            break;
        }
    }
}

Cairomotion::Cairomotion(): p1(3000, 3000, Placeholder::RED),
                            p2(3000, 3000, Placeholder::YELLOW),
                            pb2(&c1, &p2, &canvas, PopupBar::LEFT),
                            pb1(&pb2, &p1, &canvas, PopupBar::BOTTOM) {
    canvas.set_valign(Gtk::Align::CENTER);
    canvas.set_content_width(600);
    canvas.set_content_height(300);
    canvas.drawings = &drawings;
    c1.set_center_widget(canvas);

    auto style = Gtk::CssProvider::create();
    style->load_from_data(".center-container{ background-color: #ccc; }");
    Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), style, 0);
    c1.add_css_class("center-container");

    set_default_size(1300, 900);
    set_child(pb1);

    gs = Gtk::GestureStylus::create();
    canvas.setup_gesture_stylus(gs);

    gc = Gtk::GestureClick::create();
    gc->set_button(0);
    gc->signal_released().connect(sigc::mem_fun(*this, &Cairomotion::on_click));
    canvas.add_controller(gc); //it's not the window who gets the gesture, but the canvas because it must be on the same widget level where the stylus gesture

    eck = Gtk::EventControllerKey::create();
    eck->signal_key_released().connect(sigc::mem_fun(*this, &Cairomotion::on_key_released));
    add_controller(eck);


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

void Cairomotion::on_key_released(guint key, guint _, Gdk::ModifierType m_type) {
    std::cout << key << std::endl;
    switch(key) {
        case 65480: { // F11
            if (is_window_fullscreen) {
                canvas.resize(0, 0);
                unfullscreen();
                is_window_fullscreen = false;
            } else {
                fullscreen();
                is_window_fullscreen = true;
            }
            break;
        }
    }
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
    if (popup_visibility_changed) {
        popup_visibility_changed = false;
        popup_visibility_change_timer = clock->get_frame_time();
        allow_canvas_resize_just_after_toogling_popups = true;
    }
    if ((clock->get_frame_time() - popup_visibility_change_timer > 250000) &&
        allow_canvas_resize_just_after_toogling_popups) {
        canvas.resize(c1.get_width(), c1.get_height());
        allow_canvas_resize_just_after_toogling_popups = false;

    }
    return true;
}
