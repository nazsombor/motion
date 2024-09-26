//
// Created by abraham on 9/11/24.
//

#include "Timeline.h"

Frame::Frame() {
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
}

void Layer::on_click(int count, double x, double y) {
    select();
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
    int index = ((int) x) / 40;
    set_frame_index(index);
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

    append_new_layer();
}

void Timeline::resize(int width, int height) {
    header.width = 300;
    header.height = 500;
    content.width = width - 300 - vs.get_width();
    content.height = 500 - hs.get_height();
    header.queue_resize();
    content.queue_resize();
    timeline_numbers.width = content.width;
}

void Timeline::new_layer_button_on_click() {
    append_new_layer();
}

void Timeline::append_new_layer() {
    auto layer = new Layer(next_layer_index++, this);

    layer->select();

    layers.push_back(layer);
    content.container.append(layer->background);
    header.container.append(layer->header);
}

void Timeline::step_forward() {
    frame_index++;
    timeline_numbers.set_frame_index(frame_index);
}

void Timeline::step_backward() {
    if (frame_index > 0) {
        frame_index--;
        timeline_numbers.set_frame_index(frame_index);
    }
}
