//
// Created by abraham on 9/11/24.
//

#include "Timeline.h"

MoveLayer::MoveLayer() {
    set_draw_func(sigc::mem_fun(*this, &MoveLayer::on_draw));
    set_content_width(50);
    set_content_height(80);
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

Layer::Layer() {
    set_orientation(Gtk::Orientation::HORIZONTAL);
    append(ml);
}

LayerHeader::LayerHeader(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v) : Gtk::Viewport(h, v) {
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

LayerContent::LayerContent(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v) : Gtk::Viewport(h, v) {
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

Timeline::Timeline() : header(layer_header_h_adjustment, layer_v_adjustment), content(layer_content_h_adjustment, layer_v_adjustment) {
    set_orientation(Gtk::Orientation::VERTICAL);

    button_container.set_orientation(Gtk::Orientation::HORIZONTAL);
    button_container.append(add_layer_button);
    button_container.append(add_inbetween_button);
    add_layer_button.set_label("Add Layer");
    add_inbetween_button.set_label("Add In-between");
    append(button_container);

    layer_container.set_orientation(Gtk::Orientation::HORIZONTAL);
    layer_container.append(header);
    layer_container.append(content_container);
    layer_container.append(vs);
    append(layer_container);

    vs.set_orientation(Gtk::Orientation::VERTICAL);

    content_container.set_orientation(Gtk::Orientation::VERTICAL);
    content_container.append(content);
    content_container.append(hs);

}

void Timeline::resize(int width, int height) {
    header.width = 300;
    header.height = 500;
    content.width = width - 300 - vs.get_width();
    content.height = 500 - hs.get_height();
    header.queue_resize();
    content.queue_resize();
}
