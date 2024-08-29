//
// Created by abraham on 8/14/24.
//

#include "Drawings.h"

int Drawings::index(ink::stroke_model::Result &a, ink::stroke_model::Result &b, int stride, int j, int x, int y) {
    double dX = (b.position.x - a.position.x) / 5;
    double dY = (b.position.y - a.position.y) / 5;
    double dP = (b.pressure - a.pressure) / 5;


    if (a.position.x < 0) {
        a.position.x = 0;
    }

    if (a.position.y < 0) {
        a.position.y = 0;
    }

    if (b.position.x < 0) {
        b.position.x = 0;
    }

    if (b.position.y < 0) {
        b.position.y = 0;
    }

    if (a.position.x + x < 0 || b.position.x + x < 0) {
        x = 0;
    }
    if (a.position.y + y < 0 || b.position.y + y < 0) {
        y = 0;
    }

    if (a.position.x >= 1919) {
        a.position.x = 1919;
    }

    if (a.position.y >= 1079) {
        a.position.y = 1079;
    }

    if (b.position.x >= 1919) {
        b.position.x = 1919;
    }

    if (b.position.y >= 1079) {
        b.position.y = 1079;
    }

    if (a.position.x + x >= 1919 || b.position.x + x > 1919) {
        x = 0;
    }
    if (a.position.y + y >= 1079 || b.position.y + y >= 1079) {
        y = 0;
    }

    return (int)(a.position.y + y + j * dY) * stride + (int) (a.position.x + x + j * dX) * 4;

}

Drawings::Drawings() {
    surface = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
}

void Drawings::on_draw(const std::shared_ptr<Cairo::Context> &cr, int width, int height) {
    auto w = (double) width / 1920;
    auto h = (double) height / 1080;
    cr->scale(w, h);
    cr->set_source(surface, 0, 0);
    cr->paint();
}

void Drawings::pen(std::vector<ink::stroke_model::Result> &stroke) {
    auto cr = Cairo::Context::create(surface);

    for (int i = 1; i < stroke.size(); i++) {
        auto a = stroke[i - 1];
        auto b = stroke[i];
        double dX = (b.position.x - a.position.x) / 5;
        double dY = (b.position.y - a.position.y) / 5;
        double dP = (b.pressure - a.pressure) / 5;

        for (int j = 0; j < 5; j++) {
            cr->arc( a.position.x + j * dX, a.position.y + j * dY, (a.pressure + j * dP) * 5, 0, 2 * M_PI);
            cr->fill();
        }
    }
}

void Drawings::pencil(std::vector<ink::stroke_model::Result> &stroke) {

    surface->flush();
    int width = surface->get_width();
    int height = surface->get_height();
    unsigned char *data = surface->get_data();
    int stride = surface->get_stride();


    for (int i = stroke_index + 1; i < stroke.size(); i++) {
        auto a = stroke[i - 1];
        auto b = stroke[i];

        for (int j = 0; j < 5; j++) {
            double jp = a.pressure + (b.pressure - a.pressure) / 5 * j;
            int idx = index(a, b, stride, j, 0, 0);

            data[idx] = 0x00;
            data[idx + 1] = 0x00;
            data[idx + 2] = 0x00;
            data[idx + 3] = (int) data[idx + 3] + 0x44 > 0xff ? 0xff : data[idx + 3] + 0x44;;

            if (jp > .3) {
                int i2 = index(a, b, stride, j, 0, -1);
                data[i2] = 0x00;
                data[i2 + 1] = 0x00;
                data[i2 + 2] = 0x00;
                data[i2 + 3] = (int) data[i2 + 3] + 0x11 > 0xff ? 0xff : data[i2 + 3] + 0x11;
                int i3 = index(a, b, stride, j, 1, 0);
                data[i3] = 0x00;
                data[i3 + 1] = 0x00;
                data[i3 + 2] = 0x00;
                data[i3 + 3] = (int) data[i2 + 3] + 0x11 > 0xff ? 0xff : data[i2 + 3] + 0x11;
            }

            if (jp > .6) {
                int i2 = index(a, b, stride, j, 0, -1);
                data[i2] = 0x00;
                data[i2 + 1] = 0x00;
                data[i2 + 2] = 0x00;
                data[i2 + 3] = (int) data[i2 + 3] + 0x11 > 0xff ? 0xff : data[i2 + 3] + 0x11;
                int i3 = index(a, b, stride, j, 1, 0);
                data[i3] = 0x00;
                data[i3 + 1] = 0x00;
                data[i3 + 2] = 0x00;
                data[i3 + 3] = (int) data[i2 + 3] + 0x11 > 0xff ? 0xff : data[i2 + 3] + 0x11;
                int i4 = index(a, b, stride, j, 0, 1);
                data[i4] = 0x00;
                data[i4 + 1] = 0x00;
                data[i4 + 2] = 0x00;
                data[i4 + 3] = (int) data[i2 + 3] + 0x11 > 0xff ? 0xff : data[i2 + 3] + 0x11;
                int i5 = index(a, b, stride, j, -1, 0);
                data[i5] = 0x00;
                data[i5 + 1] = 0x00;
                data[i5 + 2] = 0x00;
                data[i5 + 3] = (int) data[i2 + 3] + 0x11 > 0xff ? 0xff : data[i2 + 3] + 0x11;
            }
        }
    }

    stroke_index = stroke.size() - 1;
    surface->mark_dirty(0, 0, width,height);
}

void Drawings::clear_pencil_data() {
    stroke_index = 0;
}
