//
// Created by abraham on 8/14/24.
//

#include "Canvas.h"

Canvas::Canvas() {
    set_draw_func(sigc::mem_fun(*this, &Canvas::on_draw));
}

void Canvas::resize(int width, int height) {
    auto cR = (float) width / canvas_width;

    auto w = canvas_width * cR;
    auto h = canvas_height * cR;

    if (h > height) {
        cR = (float) height / canvas_height;
        w = canvas_width * cR;
        h = canvas_height * cR;
    }

    set_content_width(w);
    set_content_height(h);
}

void Canvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height) {
    cr->set_source_rgba(1, 1, 1, 1);
    cr->rectangle(0, 0, width, height);
    cr->fill();
}
