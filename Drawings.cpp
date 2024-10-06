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

    return (int) (a.position.y + y + j * dY) * stride + (int) (a.position.x + x + j * dX) * 4;
}

Drawings::Drawings(Tools &tools) {
    this->tools = &tools;
}

void Drawings::on_draw(const std::shared_ptr<Cairo::Context> &cr, int width, int height) {
    auto w = (double) width / 1920;
    auto h = (double) height / 1080;

    cr->scale(w, h);
    if (bottom) {
        cr->set_source(bottom, 0, 0);
        cr->paint();
    }
    if (!play) {
        if (onion_skin) {
            cr->set_source(onion_skin, 0, 0);
            cr->paint();
        }
    }
    if (surface) {
        cr->set_source(surface2, 0, 0);
        cr->paint();
        cr->set_source(surface, 0, 0);
        cr->paint();
    }
    if (top) {
        cr->set_source(top, 0, 0);
        cr->paint();
    }

}

void Drawings::pen(std::vector<ink::stroke_model::Result> &stroke) {
    auto cr = Cairo::Context::create(surface);

    for (int i = stroke_index + 1; i < stroke.size(); i++) {
        auto a = stroke[i - 1];
        auto b = stroke[i];
        double dX = (b.position.x - a.position.x) / 5;
        double dY = (b.position.y - a.position.y) / 5;
        double dP = (b.pressure - a.pressure) / 5;

        for (int j = 0; j < 5; j++) {
            cr->arc(a.position.x + j * dX, a.position.y + j * dY, (a.pressure + j * dP) * (a.pressure + j * dP) * 2, 0,
                    2 * M_PI);
            cr->fill();
        }
    }

    stroke_index = stroke.size() - 1;

    std::cout << "pen " << stroke.size() << std::endl;
}

void Drawings::pencil(std::vector<ink::stroke_model::Result> &stroke) {
    surface->flush();
    int width = surface->get_width();
    int height = surface->get_height();
    unsigned char *data = surface->get_data();
    int stride = surface->get_stride();

    std::cout << "pencil " << stroke.size() << std::endl;

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
    surface->mark_dirty(0, 0, width, height);
}

void Drawings::solid_brush(std::vector<ink::stroke_model::Result> &stroke) {
    double size = 50.0;

    auto cr = Cairo::Context::create(surface2);

    for (int i = stroke_index; i < stroke.size(); i++) {
        auto a = stroke[i];

        cr->set_source_rgb(tools->color_picker.r, tools->color_picker.g, tools->color_picker.b);
        cr->arc(a.position.x, a.position.y, a.pressure * a.pressure * size, 0, 2 * M_PI);
        cr->fill();
    }

    stroke_index = stroke.size() - 1;

    std::cout << "solid brush" << stroke.size() << std::endl;
}

void Drawings::clear_stroke_data() {
    stroke_index = 0;
}

void Drawings::play_next() {

}

class FillArea {
public:
    int x, y;
    int please_continue;

    FillArea(int x, int y, int p_continue) : x(x), y(y), please_continue(p_continue) {
    }
};

void Drawings::fill_area(int x, int y) {
    surface2->flush();

    unsigned char check_color[4], fill_color[4] = {
        (unsigned char) (tools->color_picker.b * 256), (unsigned char) (tools->color_picker.g * 256),
        (unsigned char) (tools->color_picker.r * 256), 255
    };
    std::memcpy(check_color, pixel2(x, y), sizeof(check_color));

    std::vector<FillArea> points;
    points.emplace_back(x, y, 0);


    while (!points.empty()) {
        for (int i = points.size() - 1; i >= 0; i--) {
            auto p = pixel2(points[i].x, points[i].y);

            if (p[0] == fill_color[0] && p[1] == fill_color[1] && p[2] == fill_color[2] && p[3] == fill_color[3]) {
                points.erase(points.begin() + i);
                continue;
            }

            p[0] = fill_color[0];
            p[1] = fill_color[1];
            p[2] = fill_color[2];
            p[3] = fill_color[3];

            if (points[i].please_continue > 1) {
                points.erase(points.begin() + i);
                continue;
            }

            bool up = points[i].y - 1 >= 0, down = points[i].y + 1 < 1080, left = points[i].x - 1 > 0, right =
                    points[i].x + 1 < 1920;


            auto a = pixel2(points[i].x + 1, points[i].y);
            auto b = pixel2(points[i].x, points[i].y + 1);
            auto c = pixel2(points[i].x, points[i].y - 1);
            auto d = pixel2(points[i].x - 1, points[i].y);

            auto a2 = pixel(points[i].x + 1, points[i].y);
            auto b2 = pixel(points[i].x, points[i].y + 1);
            auto c2 = pixel(points[i].x, points[i].y - 1);
            auto d2 = pixel(points[i].x - 1, points[i].y);


            if (right) {
                if (a[0] == check_color[0] && a[1] == check_color[1] && a[2] == check_color[2] && a[3] == check_color[
                        3] && a2[3] == 0 && points[i].please_continue == 0) {
                    points.emplace_back(points[i].x + 1, points[i].y, 0);
                } else {
                    auto j = 1;
                    if (points[i].please_continue <= 1) {
                        if (points[i].please_continue == 1) {
                            j = 2;
                        }
                        points.emplace_back(points[i].x + 1, points[i].y, j);
                    }
                }
            }

            if (down) {
                if (b[0] == check_color[0] && b[1] == check_color[1] && b[2] == check_color[2] && b[3] == check_color[
                        3] && b2[3] == 0 && points[i].please_continue == 0) {
                    if (points[i].y + 1 < 1080)
                        points.emplace_back(points[i].x, points[i].y + 1, 0);
                } else {
                    auto j = 1;
                    if (points[i].please_continue <= 1) {
                        if (points[i].please_continue == 1) {
                            j = 2;
                        }

                        points.emplace_back(points[i].x, points[i].y + 1, j);
                    }
                }
            }

            if (up) {
                if (c[0] == check_color[0] && c[1] == check_color[1] && c[2] == check_color[2] && c[3] == check_color[
                        3] && c2[3] == 0 && points[i].please_continue == 0) {
                    if (points[i].y - 1 >= 0)
                        points.emplace_back(points[i].x, points[i].y - 1, 0);
                } else {
                    auto j = 1;
                    if (points[i].please_continue <= 1) {
                        if (points[i].please_continue == 1) {
                            j = 2;
                        }
                        points.emplace_back(points[i].x, points[i].y - 1, j);
                    }
                }
            }

            if (left) {
                if (d[0] == check_color[0] && d[1] == check_color[1] && d[2] == check_color[2] && d[3] == check_color[
                        3] && d2[3] == 0 && points[i].please_continue == 0) {
                    points.emplace_back(points[i].x - 1, points[i].y, 0);
                } else {
                    auto j = 1;
                    if (points[i].please_continue <= 1) {
                        if (points[i].please_continue == 1) {
                            j = 2;
                        }
                        points.emplace_back(points[i].x - 1, points[i].y, j);
                    }
                }
            }

            points.erase(points.begin() + i);
        }
    }

    surface2->mark_dirty();
}

unsigned char *Drawings::pixel(int x, int y) {
    int stride = surface->get_stride();
    int index = (stride * y) + (4 * x);
    return surface->get_data() + index;
}

unsigned char *Drawings::pixel2(int x, int y) {
    int stride = surface2->get_stride();
    int index = (stride * y) + (4 * x);
    return surface2->get_data() + index;
}

void Drawings::calculate_onion_skin() {

    if (!onion_skin) return;

    onion_skin->flush();

    for (int i = 0; i < onion_skin->get_stride() * onion_skin->get_height(); i += 4) {
        unsigned char *data = &onion_skin->get_data()[i];
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;

        if (previous_surface) {
            auto d2 = &previous_surface->get_data()[i];
            if (d2[3] != 0) {
                data[0] = 255;
                data[3] = 100;
            }
        }
        if (next_surface) {
            auto d2 = &next_surface->get_data()[i];
            if (d2[3] != 0) {
                data[1] = 255;
                data[3] = 100;
            }
        }
    }

    onion_skin->mark_dirty(0, 0, onion_skin->get_width(), onion_skin->get_height());
}
