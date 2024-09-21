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

Timeline::Timeline() {
    set_orientation(Gtk::Orientation::VERTICAL);
    auto l = new Layer();
    layers.push_back(l);
    append(*l);
}
