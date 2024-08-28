//
// Created by abraham on 8/14/24.
//

#include "Drawings.h"

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
    std::cout << "Stroke" << std::endl;
    // Glib::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

    surface->flush();
    int width = surface->get_width();
    int height = surface->get_height();
    unsigned char *data = surface->get_data();
    int stride = surface->get_stride();


    for (int i = 1; i < stroke.size(); i++) {
        auto a = stroke[i - 1];
        auto b = stroke[i];

        double dX = (b.position.x - a.position.x) / 5;
        double dY = (b.position.y - a.position.y) / 5;
        double dP = (b.pressure - a.pressure) / 5;

        for (int j = 0; j < 5; j++) {

            data[(int)(a.position.y + j * dY) * stride + (int) (a.position.x + j * dX) * 4] = 0x00;
            data[(int)(a.position.y + j * dY) * stride + (int) (a.position.x + j * dX) * 4 + 1] = 0x00;
            data[(int)(a.position.y + j * dY) * stride + (int) (a.position.x + j * dX) * 4 + 2] = 0x00;
            data[(int)(a.position.y + j * dY) * stride + (int) (a.position.x + j * dX) * 4 + 3] = 0xff;
        }


    }
    surface->mark_dirty(0, 0, width,height);
    surface->flush();
}
