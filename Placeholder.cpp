//
// Created by abraham on 8/12/24.
//

#include "Placeholder.h"

Placeholder::Placeholder(int width, int height, Color color) {
    set_content_width(width);
    set_content_height(height);
    set_draw_func(sigc::mem_fun(*this, &Placeholder::on_draw));
    this->color = color;
}

void Placeholder::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height) {
    switch (color) {
        case RED:
            cr->set_source_rgba(1, 0, 0, 1);
            break;
        case GREEN:
            cr->set_source_rgba(0, 1, 0, 1);
            break;
        case BLUE:
            cr->set_source_rgba(0, 0, 1, 1);
            break;
        case YELLOW:
            cr->set_source_rgba(255/256.0, 191/256.0, 0/256.0, 1);
            break;
        case WHITE:
            cr->set_source_rgba(1, 1, 1, 1);
    }
    cr->rectangle(0, 0, width, height);
    cr->fill();
}
