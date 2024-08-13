//
// Created by abraham on 8/12/24.
//

#include "Cairomotion.h"

void Cairomotion::on_click(int type, double x, double y) {
    auto button = gc->get_current_event()->get_button();
    switch(button) {
        case GDK_BUTTON_MIDDLE: {
            std::cout << "Middle stylus button" << std::endl;
            canvas_is_maximized = !canvas_is_maximized;
            if (canvas_is_maximized) {
                auto width = get_width();
                auto height = get_height();
                p2.set_content_width(width - 30);
                p2.set_content_height(height - 30);
            } else {
                p2.set_content_width(1920 / 10 *3);
                p2.set_content_height(1080 / 10 * 3);
            }
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

Cairomotion::Cairomotion(): p1(30, 30, Placeholder::RED), p2(30, 30, Placeholder::WHITE),
                            p3(30, 30, Placeholder::YELLOW),
                            pb2(&c1, &p3, PopupBar::LEFT),
                            pb1(&pb2, &p1, PopupBar::BOTTOM) {
    p2.set_valign(Gtk::Align::CENTER);
    p2.set_content_width(600);
    p2.set_content_height(300);
    c1.set_center_widget(p2);

    auto style = Gtk::CssProvider::create();
    style->load_from_data(".center-container{ background-color: #ccc }");
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
