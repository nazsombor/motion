//
// Created by abraham on 8/14/24.
//

#include "Drawings.h"

Frame::Frame() {
    surface = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920,1080);
    surface2 = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920,1080);
}

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

Drawings::Drawings(Tools &tools) {
    this->tools = &tools;
    frames.emplace_back();
}

void Drawings::on_draw(const std::shared_ptr<Cairo::Context> &cr, int width, int height) {
    auto w = (double) width / 1920;
    auto h = (double) height / 1080;
    cr->scale(w, h);
    cr->set_source(frames[frameIndex].surface2, 0, 0);
    cr->paint();
    cr->set_source(frames[frameIndex].surface, 0, 0);
    cr->paint();
}

void Drawings::pen(std::vector<ink::stroke_model::Result> &stroke) {
    auto cr = Cairo::Context::create(frames[frameIndex].surface);

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

    frames[frameIndex].surface->flush();
    int width = frames[frameIndex].surface->get_width();
    int height = frames[frameIndex].surface->get_height();
    unsigned char *data = frames[frameIndex].surface->get_data();
    int stride = frames[frameIndex].surface->get_stride();


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

            if (jp > .7) {
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
    frames[frameIndex].surface->mark_dirty(0, 0, width,height);
}

void Drawings::solid_brush(std::vector<ink::stroke_model::Result> &stroke) {

    double size = 100.0;

    auto cr = Cairo::Context::create(frames[frameIndex].surface2);

    for (int i = stroke_index; i < stroke.size(); i++) {
        auto a = stroke[i];

        cr->set_source_rgb(tools->color_picker.r, tools->color_picker.g, tools->color_picker.b);
        cr->arc( a.position.x, a.position.y, a.pressure * a.pressure * size, 0, 2 * M_PI);
        cr->fill();
    }

    stroke_index = stroke.size() - 1;
}

void Drawings::clear_pencil_data() {
    stroke_index = 0;
}

void Drawings::step_forward() {
    if (frameIndex < frames.size() - 1) {
        frameIndex++;
    } else {
        frameIndex++;
        frames.emplace_back();
    }
}

void Drawings::step_backward() {
    if (frameIndex > 0) {
        frameIndex--;
    }
}

void Drawings::play_next() {
    if (frameIndex >= frames.size() - 1) {
        frameIndex = 0;
    } else {
        frameIndex++;
    }
}
