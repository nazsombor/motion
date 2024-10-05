//
// Created by abraham on 9/11/24.
//

#include "Timeline.h"

Frame::Frame(int index) : index(index){
    surface = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
    surface2 = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
    onion_skin = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
}

MoveLayer::MoveLayer() : gs(Gtk::GestureClick::create()){
    set_draw_func(sigc::mem_fun(*this, &MoveLayer::on_draw));
    set_content_width(50);
    set_content_height(60);
    set_valign(Gtk::Align::START);
    set_halign(Gtk::Align::START);

    gs->signal_released().connect(sigc::mem_fun(*this, &MoveLayer::on_click));

    add_controller(gs);
}

void MoveLayer::on_draw(const Glib::RefPtr<Cairo::Context> &ctx, int width, int height) {
    ctx->set_source_rgb(0.5, 0.5, 0.5);
    ctx->move_to(width / 2, 10);
    ctx->line_to(width - 10, height / 2 - 10);
    ctx->line_to(10, height / 2 - 10);
    ctx->fill();

    ctx->move_to(width / 2, height - 10);
    ctx->line_to(width - 10, height / 2 + 10);
    ctx->line_to(10, height / 2 + 10);
    ctx->fill();
}

void MoveLayer::on_click(int count, double x, double y) {

}

Layer::Layer(int index, Timeline *timeline) : index(index) {
    background.set_draw_func(sigc::mem_fun(*this, &Layer::on_draw));
    background.set_content_height(60);
    background.set_content_width(3000);
    background.set_valign(Gtk::Align::START);
    header.set_orientation(Gtk::Orientation::HORIZONTAL);
    header.append(ml);
    header.append(label);

    gc = Gtk::GestureClick::create();
    gc->signal_released().connect(sigc::mem_fun(*this, &Layer::on_click));
    header.add_controller(gc);

    this->timeline = timeline;
}

void Layer::on_draw(const Glib::RefPtr<Cairo::Context> &ctx, int width, int height) {
    for (int i = 0; i < width; i += 40) {
        if (i % 80 == 0) {
            ctx->set_source_rgb(0.5, 0.5, 0.5);
        } else {
            ctx->set_source_rgb(0.7, 0.7, 0.7);
        }
        ctx->rectangle(i, 0, 40, 60);
        ctx->fill();
    }

    for (auto frame : frames) {
        ctx->set_source_rgb(0.9, 0.9, 0.9);
        ctx->rectangle(frame->index * 40, 0, frame->duration * 40, 60);
        ctx->fill_preserve();
        ctx->set_source_rgb(0, 0, 0);
        ctx->stroke();
    }

    ctx->set_source_rgb(0, 0, 0);
    ctx->rectangle(0, 0, width, height);
    ctx->stroke();
}

void Layer::deselect() {
    header.remove_css_class("selected-layer");
}

void Layer::select() {
    for (auto l : timeline->layers) {
        l->deselect();
    }
    header.add_css_class("selected-layer");
    timeline->layer_index = index;
}

void Layer::on_click(int count, double x, double y) {
    select();
}

Frame *Layer::get_frame(int frame_index) {
    auto f = frames.size();
    for(auto frame : frames) {
        if (frame->index == frame_index || (frame->index + frame->duration > frame_index && frame->index < frame_index)) {
            return frame;
        }
    }
    return nullptr;
}

Frame * Layer::get_previous_frame(Frame *frame) {
    Frame *previous_frame = nullptr;
    for (auto f : frames) {
        if (f == frame) {
            return previous_frame;
        }
        previous_frame = f;
    }
    return nullptr;
}

Frame *Layer::get_previous_frame(int frame_index) {
    Frame *previous_frame = nullptr;

    for (auto frame : frames) {
        if (frame->index + frame->duration - 1 < frame_index) {
            previous_frame = frame;
        }
    }

    return previous_frame;
}

Frame * Layer::get_next_frame(Frame *frame) {
    Frame *next_frame = nullptr;
    for (int i = frames.size() - 1; i >= 0; i--) {
        if (frames[i] == frame) {
            return next_frame;
        }
        next_frame = frames[i];
    }
    return nullptr;
}

Frame * Layer::get_next_frame(int frame_index) {

    Frame *next_frame = nullptr;

    if (frames.size() == 0) return nullptr;

    for (int i = frames.size() - 1; i >= 0; i--) {
        if (frames[i]->index > frame_index) {
            next_frame = frames[i];
        }
    }

    return next_frame;
}

int Layer::get_last_frame_index() {
    auto frame = frames[frames.size() - 1];
    return frame->index + frame->duration - 1;
}

LayerHeader::LayerHeader(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v) : Gtk::Viewport(h, v), top_margin(20, 20, Placeholder::WHITE) {
    set_child(container);
    container.set_orientation(Gtk::Orientation::VERTICAL);
    container.append(top_margin);
}

Gtk::SizeRequestMode LayerHeader::get_request_mode_vfunc() const {
    return Gtk::SizeRequestMode::CONSTANT_SIZE;
}

void LayerHeader::measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural,
                                int &minimum_baseline, int &natural_baseline) const {
    minimum_baseline = natural_baseline = -1;

    if (orientation == Gtk::Orientation::HORIZONTAL) {
        minimum = width;
        natural = width;
    } else {
        minimum = height;
        natural = height;
    }
}

LayerContent::LayerContent(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v) : Gtk::Viewport(h, v), gs(Gtk::GestureStylus::create()){
    set_child(container);
    container.set_orientation(Gtk::Orientation::VERTICAL);
    gs->signal_down().connect(sigc::mem_fun(*this, &LayerContent::on_stylus_down));
    gs->signal_motion().connect(sigc::mem_fun(*this, &LayerContent::on_stylus_motion));
    gs->signal_up().connect(sigc::mem_fun(*this, &LayerContent::on_stylus_up));
    container.add_controller(gs);
}

Gtk::SizeRequestMode LayerContent::get_request_mode_vfunc() const {
    return Gtk::SizeRequestMode::CONSTANT_SIZE;
}

void LayerContent::measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural,
                                 int &minimum_baseline, int &natural_baseline) const {
    minimum_baseline = natural_baseline = -1;

    if (orientation == Gtk::Orientation::HORIZONTAL) {
        minimum = width;
        natural = width;
    } else {
        minimum = height;
        natural = height;
    }
}

void LayerContent::on_stylus_down(double x, double y) {
    std::cout << x << ' ' << y << std::endl;

}

void LayerContent::on_stylus_motion(double x, double y) {
    std::cout << x << ' ' << y << std::endl;
}

void LayerContent::on_stylus_up(double x, double y) {
    std::cout << x << ' ' << y << std::endl;
}

TimelineNumbers::TimelineNumbers(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v) : Gtk::Viewport(h, v), gc(Gtk::GestureClick::create()) {
    set_child(frames);
    frames.set_content_width(3000);
    frames.set_content_height(20);
    frames.set_draw_func(sigc::mem_fun(*this, &TimelineNumbers::on_draw));

    gc->signal_released().connect(sigc::mem_fun(*this, &TimelineNumbers::on_click));
    add_controller(gc);

}

void TimelineNumbers::on_draw(const Glib::RefPtr<Cairo::Context> &ctx, int width, int height) {
    auto layout = Pango::Layout::create(ctx);
    Pango::FontDescription font;
    font.set_family("Sans");
    font.set_weight(Pango::Weight::NORMAL);
    font.set_absolute_size(15 * PANGO_SCALE);
    layout->set_font_description(font);

    ctx->set_source_rgb(.7, .7, .7);
    ctx->rectangle(frame_index * 40, 0, 40, 20);
    ctx->fill();

    ctx->set_source_rgb(0, 0, 0);
    for (int i = 0; i < 3000; i++) {
        ctx->move_to(i * 40 + 5, 0);
        layout->set_text(std::to_string(i));
        layout->show_in_cairo_context(ctx);
        ctx->move_to(i * 40 + 40, 0);
        ctx->line_to(i * 40 + 40, 20);
        ctx->stroke();
    }
}

Gtk::SizeRequestMode TimelineNumbers::get_request_mode_vfunc() const {
    return Gtk::SizeRequestMode::CONSTANT_SIZE;
}

void TimelineNumbers::measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural,
    int &minimum_baseline, int &natural_baseline) const {
    minimum_baseline = natural_baseline = -1;

    if (orientation == Gtk::Orientation::HORIZONTAL) {
        minimum = width;
        natural = width;
    } else {
        minimum = 20;
        natural = 20;
    }}

void TimelineNumbers::set_frame_index(int index) {
    frame_index = index;
    frames.queue_draw();
}

void TimelineNumbers::on_click(int count, double x, double y) {
    auto ratio = timeline->layer_content_h_adjustment->get_upper() / frames.get_content_width();
    int x_offset = timeline->layer_content_h_adjustment->get_value() * ratio;
    int index = ((int) x + x_offset) / 40;
    set_frame_index(index);
    timeline->set_frame_index(frame_index);
}

Timeline::Timeline() : layer_header_h_adjustment(Gtk::Adjustment::create(0, 0, 100)),
                       layer_content_h_adjustment(Gtk::Adjustment::create(0, 0, 100)),
                       not_in_use(Gtk::Adjustment::create(0, 0, 0)),
                       layer_v_adjustment(Gtk::Adjustment::create(0, 0, 100)),
                       header(layer_header_h_adjustment, layer_v_adjustment),
                       content(layer_content_h_adjustment, layer_v_adjustment),
                       timeline_numbers(layer_content_h_adjustment, not_in_use){
    set_orientation(Gtk::Orientation::VERTICAL);

    button_container.set_orientation(Gtk::Orientation::HORIZONTAL);
    button_container.append(add_layer_button);
    button_container.append(add_inbetween_button);
    add_layer_button.set_label("Add Layer");
    add_layer_button.signal_clicked().connect(sigc::mem_fun(*this, &Timeline::new_layer_button_on_click));
    add_inbetween_button.set_label("Add In-between");
    append(button_container);

    layer_container.set_orientation(Gtk::Orientation::HORIZONTAL);
    layer_container.append(header);
    layer_container.append(content_container);
    layer_container.append(vs);
    append(layer_container);

    vs.set_orientation(Gtk::Orientation::VERTICAL);
    vs.set_adjustment(layer_v_adjustment);
    hs.set_adjustment(layer_content_h_adjustment);

    content_container.set_orientation(Gtk::Orientation::VERTICAL);
    content_container.append(timeline_numbers);
    content_container.append(content);
    content_container.append(hs);

    content.drawings = drawings;
    timeline_numbers.timeline = this;

    append_new_layer();
}

void Timeline::resize(int width, int height) {
    header.width = 300;
    header.height = 500;
    content.width = width - 300 - vs.get_width();
    content.height = 500 - hs.get_height();
    timeline_numbers.width = content.width;
    header.queue_resize();
    content.queue_resize();
    timeline_numbers.queue_resize();
}

void Timeline::new_layer_button_on_click() {
    append_new_layer();
}

Layer *Timeline::append_new_layer() {
    auto layer = new Layer(next_layer_index++, this);
    layer->select();

    layers.push_back(layer);
    content.container.append(layer->background);
    header.container.append(layer->header);

    return layer;
}

void Timeline::step_forward() {
    frame_index++;
    set_frame_index(frame_index);
}

void Timeline::step_backward() {
    if (frame_index > 0) {
        frame_index--;
        set_frame_index(frame_index);
    }
}

void Timeline::draw_top_and_bottom() {
    auto frame = layers[layer_index]->get_frame(frame_index);
    auto top = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
    auto top_ctx = Cairo::Context::create(top);
    auto bottom = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
    auto bottom_ctx = Cairo::Context::create(bottom);

    bool is_bottom = true;
    for (int i = layers.size() - 1; i >= 0; i--) {
        auto f = layers[i]->get_frame(frame_index);

        if (!f) continue;

        if (i == layer_index) {
            is_bottom = false;
            continue;
        }

        if (is_bottom) {
            bottom_ctx->set_source(f->surface2, 0, 0);
            bottom_ctx->paint();
            bottom_ctx->set_source(f->surface, 0, 0);
            bottom_ctx->paint();
        } else {
            top_ctx->set_source(f->surface2, 0, 0);
            top_ctx->paint();
            top_ctx->set_source(f->surface, 0, 0);
            top_ctx->paint();
        }
    }

    drawings->top = top;
    drawings->bottom = bottom;

    request_canvas_redraw = true;
}

void Timeline::check_if_frame_exists() {
    auto frame = layers[layer_index]->get_frame(frame_index);

    if (!frame) {
        frame = new Frame(frame_index);
        layers[layer_index]->frames.push_back(frame);
        auto previous_frame = layers[layer_index]->get_previous_frame(frame);
        if (previous_frame) {
            previous_frame->duration = frame_index - previous_frame->index;
        }
    }

    Cairo::RefPtr<Cairo::ImageSurface> top;
    Cairo::RefPtr<Cairo::ImageSurface> bottom;

    draw_top_and_bottom();

    drawings->surface = frame->surface;
    drawings->surface2 = frame->surface2;
    drawings->onion_skin = frame->onion_skin;

    drawings->calculate_onion_skin();

    layers[layer_index]->background.queue_draw();

}

void Timeline::set_frame_index(int index) {
    timeline_numbers.set_frame_index(index);
    frame_index = index;
    request_canvas_redraw = true;
    auto frame = layers[layer_index]->get_frame(frame_index);

    if (frame) {
        drawings->surface = frame->surface;
        drawings->surface2 = frame->surface2;
        drawings->onion_skin = frame->onion_skin;
    } else {
        drawings->surface = nullptr;
    }

    draw_top_and_bottom();

    auto previous_frame = layers[layer_index]->get_previous_frame(frame_index);
    if (previous_frame) {
        drawings->previous_surface = previous_frame->surface;
    } else {
        drawings->previous_surface = nullptr;
    }

    auto next_frame = layers[layer_index]->get_next_frame(frame_index);
    if (next_frame) {
        drawings->next_surface = next_frame->surface;
    } else {
        drawings->next_surface = nullptr;
    }
    drawings->calculate_onion_skin();

}

void Timeline::play_next() {

    int last_frame_index = 0;
    for (auto layer : layers) {
        int lfi = layer->get_last_frame_index();
        if (lfi > last_frame_index) {
            last_frame_index = lfi;
        }
    }

    if (last_frame_index > frame_index) {
        set_frame_index(frame_index + 1);
    } else {
        set_frame_index(0);
    }
}

void Timeline::clear_layers() {

    for (auto layer : layers) {
        content.container.remove(layer->background);
        header.container.remove(layer->header);
    }

    layer_index = 0;
    next_layer_index = 0;

    layers.clear();

}

void Timeline::export_to(const std::string &path) {
    int last_frame_index = 0;
    for (auto layer : layers) {
        auto l = layer->get_last_frame_index();
        if (l > last_frame_index) {
            last_frame_index = l;
        }
    }
    int index = 0;
    while (index <= last_frame_index) {
        auto frame = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
        auto cr = Cairo::Context::create(frame);

        for (int i = layers.size() - 1; i >= 0; i--) {
            auto f = layers[i]->get_frame(index);
            if (f) {
                cr->set_source(f->surface2, 0, 0);
                cr->paint();
                cr->set_source(f->surface, 0, 0);
                cr->paint();
            }
        }

        std::string name = std::to_string(index);

        for (int i = 4 - name.size(); i >= 0; i--) {
            name.insert(0, "0");
        }

        frame->write_to_png(path + "_" + name + ".png");

        index++;
    }
}
