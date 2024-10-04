//
// Created by abraham on 8/12/24.
//

#include "Cairomotion.h"

Cairomotion::Cairomotion(): drawings(tools),
                            pb2(&container, &tools, &canvas, PopupBar::LEFT),
                            pb1(&pb2, &timeline, &canvas, PopupBar::BOTTOM) {
    canvas.set_valign(Gtk::Align::CENTER);
    canvas.drawings = &drawings;
    timeline.drawings = &drawings;
    canvas.timeline = &timeline;
    container.set_center_widget(canvas);

    auto style = Gtk::CssProvider::create();
    style->load_from_data(R"(
        .center-container { background-color: #ccc; }
        .selected-tool { background: #00f; color: white;}
        .selected-layer { background-color: #ccc; }
)");
    Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), style, 0);
    container.add_css_class("center-container");
    tools.pen.get_style_context()->add_provider(style, GTK_STYLE_PROVIDER_PRIORITY_USER);
    tools.pencil.get_style_context()->add_provider(style, GTK_STYLE_PROVIDER_PRIORITY_USER);
    tools.solid_brush.get_style_context()->add_provider(style, GTK_STYLE_PROVIDER_PRIORITY_USER);
    tools.textured_brush.get_style_context()->add_provider(style, GTK_STYLE_PROVIDER_PRIORITY_USER);
    tools.color_list.get_style_context()->add_provider(style,GTK_STYLE_PROVIDER_PRIORITY_USER);
    timeline.content.container.get_style_context()->add_provider(style, GTK_STYLE_PROVIDER_PRIORITY_USER);

    gs = Gtk::GestureStylus::create();
    canvas.setup_gesture_stylus(gs);

    gc = Gtk::GestureClick::create();
    gc->set_button(0);
    gc->signal_released().connect(sigc::mem_fun(*this, &Cairomotion::on_click));
    //it's not the window who gets the gesture, but the canvas because it must be on the same widget level where the stylus gesture
    canvas.add_controller(gc);

    eck = Gtk::EventControllerKey::create();
    eck->signal_key_released().connect(sigc::mem_fun(*this, &Cairomotion::on_key_released));
    add_controller(eck);

    set_default_size(1300, 900);
    set_child(pb1);
    add_tick_callback(sigc::mem_fun(*this, &Cairomotion::tick));
}

void Cairomotion::on_click(int type, double x, double y) {
    auto button = gc->get_current_event()->get_button();

    switch (button) {
        case GDK_BUTTON_MIDDLE: {
            auto r = 1920.0 / canvas.get_width();
            if (tools.solid_brush_selected) {
                int x_int = r * x;
                int y_int = r * y;
                drawings.fill_area(x_int, y_int);
                canvas.queue_draw();
            }
            canvas.stylus_up_is_not_primary_button = true;
            break;
        }
        case GDK_BUTTON_PRIMARY: {
            canvas.stylus_up_is_not_primary_button = false;
            break;
        }
        case GDK_BUTTON_SECONDARY: {
            pb1.toogle_bar_visibility();
            pb2.toogle_bar_visibility();
            popup_visibility_changed = true;
            canvas.stylus_up_is_not_primary_button = true;
            break;
        }
    }
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
    switch (key) {
        case 65480: {
            // F11
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
        case 65363: {
            //Right arrow
            canvas.queue_draw();
            timeline.step_forward();
            break;
        }
        case 65361: {
            //Left arrow
            canvas.queue_draw();
            timeline.step_backward();
            break;
        }
        case 32: {
            //Space
            drawings.play = !drawings.play;
            break;
        }
        case 112: {
            tools.current_color.pick_button_clicked = true;
            break;
        }
    }
}

void Cairomotion::handle_window_resize(const Glib::RefPtr<Gdk::FrameClock> &clock) {
    if (start_window_size_change) {
        window_size_change_timer = clock->get_frame_time();
        start_window_size_change = false;
        allow_canvas_resize_once_per_window_resize = true;
    }
    if ((clock->get_frame_time() - window_size_change_timer) > 250000 &&
        allow_canvas_resize_once_per_window_resize) {
        canvas.resize(container.get_width(), container.get_height());
        timeline.resize(get_width(), get_height());
        allow_canvas_resize_once_per_window_resize = false;
    }
    if (popup_visibility_changed) {
        popup_visibility_changed = false;
        popup_visibility_change_timer = clock->get_frame_time();
        allow_canvas_resize_just_after_toggling_popups = true;
    }
    if ((clock->get_frame_time() - popup_visibility_change_timer > 250000) &&
        allow_canvas_resize_just_after_toggling_popups) {
        canvas.resize(container.get_width(), container.get_height());
        allow_canvas_resize_just_after_toggling_popups = false;
    }
}

void Cairomotion::handle_play(const Glib::RefPtr<Gdk::FrameClock> &clock) {
    if (drawings.play) {
        drawings.stop_playing = true;
        if (clock->get_frame_time() - drawings.previous_frame_time > drawings.frame_duration) {
            drawings.previous_frame_time = clock->get_frame_time();
            timeline.play_next();
            timeline.timeline_numbers.queue_draw();
            canvas.queue_draw();
        }
    } else if (drawings.stop_playing) {
        drawings.stop_playing = false;
        canvas.queue_draw();
    }
}

void Cairomotion::handle_update_color_picker() {
    if (tools.color_list.update_color_picker) {
        tools.color_list.update_color_picker = false;
        tools.color_picker.r = tools.color_list.r;
        tools.color_picker.g = tools.color_list.g;
        tools.color_picker.b = tools.color_list.b;
        tools.color_picker.x = tools.color_list.r * 255;
        tools.color_picker.y = tools.color_list.g * 255;
        tools.color_picker.adjustment->set_value(tools.color_list.b * 255);
        tools.color_picker.drawing_area.queue_draw();
    }
}

void Cairomotion::handle_pick_color_from_anywhere_the_screen() {
    if (tools.current_color.pick_button_clicked) {
        XQueryPointer(
            tools.current_color.display,
            tools.current_color.window,
            &tools.current_color.event.xbutton.root,
            &tools.current_color.event.xbutton.subwindow,
            &tools.current_color.event.xbutton.x_root,
            &tools.current_color.event.xbutton.y_root,
            &tools.current_color.event.xbutton.x,
            &tools.current_color.event.xbutton.y,
            &tools.current_color.event.xbutton.state
        );

        if (tools.current_color.event.xbutton.state & 256) {
            tools.current_color.pick_button_clicked = false;
            XImage *image1;
            image1 = XGetImage(tools.current_color.display, tools.current_color.window, 0, 0,
                               tools.current_color.attributes.width, tools.current_color.attributes.height,
                               AllPlanes, ZPixmap);

            XColor color;
            color.pixel = XGetPixel(image1, tools.current_color.event.xbutton.x,
                                    tools.current_color.event.xbutton.y);
            XQueryColor(tools.current_color.display,
                        XDefaultColormap(tools.current_color.display, 0), &color);
            std::cout << color.red / 256 << " " << color.green / 256 << " " << color.blue / 256 << std::endl;

            XFree(image1);

            tools.color_picker.update(color.red / 256, color.green / 256, color.blue / 256);
        }
    }
}


void Cairomotion::handle_save() {
    if (tools.file_operation.start_saving) {
        tools.file_operation.start_saving = false;

        std::vector<LayerEntity> layers;

        for (auto layer : timeline.layers) {
            LayerEntity layer_entity(layer->index);

            for (auto frame : layer->frames) {
                layer_entity.frames.emplace_back(frame->index, frame->duration, frame->surface, frame->surface2);
            }
            layers.push_back(layer_entity);
        }

        std::vector<ColorEntity> colors;

        for (auto item : tools.color_list.items) {
            ColorEntity color(item->r, item->g, item->b, item->text.get_text());
            colors.push_back(color);
        }

        tools.file_operation.save(layers, colors);
    }
}

void Cairomotion::handle_open() {
    if (tools.file_operation.start_opening) {
        tools.file_operation.start_opening = false;

        timeline.clear_layers();

        std::vector<LayerEntity> layers;
        std::vector<ColorEntity> colors;

        tools.file_operation.open(layers, colors);

        for (auto layer : layers) {
            auto l = timeline.append_new_layer();
            for (auto frame : layer.frames) {
                auto f = new Frame(frame.index);
                f->duration = frame.duration;
                f->surface = frame.surface;
                f->surface2 = frame.surface2;
                f->onion_skin = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
                l->frames.push_back(f);
            }
            l->background.queue_draw();
        }

        timeline.set_frame_index(0);

        for (auto color : colors) {
            tools.color_list.add_color(color.r, color.g, color.b, color.name);
        }
    }
}

bool Cairomotion::tick(const Glib::RefPtr<Gdk::FrameClock> &clock) {
    handle_window_resize(clock);
    handle_play(clock);
    handle_update_color_picker();
    handle_pick_color_from_anywhere_the_screen();

    if (timeline.request_canvas_redraw) {
        timeline.request_canvas_redraw = false;
        canvas.queue_draw();
    }

    handle_save();
    handle_open();

    if (tools.file_operation.start_export) {
        tools.file_operation.start_export = false;

        timeline.export_to(tools.file_operation.export_path);

    }


    return true;
}
