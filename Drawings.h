//
// Created by abraham on 8/14/24.
//

#ifndef DRAWINGS_H
#define DRAWINGS_H
#include <gtkmm.h>

#include "thirdparty/ink-stroke-modeler/ink_stroke_modeler/types.h"
#include "ui/Tools.h"

class Frame {
    public:
    Cairo::RefPtr<Cairo::ImageSurface> surface, surface2, onion_skin;
    Frame();
};


class Drawings {
    int stroke_index;

    int index(ink::stroke_model::Result &a, ink::stroke_model::Result &b, int stride, int j, int x, int y);

public:
    std::vector<Frame> frames;
    int frameIndex = 0;
    gint64 frame_duration = 1000000 / 12;
    gint64 previous_frame_time = 0;
    bool play = false, stop_playing = false;
    Tools * tools;

    Drawings(Tools &tools);

    void on_draw(const std::shared_ptr<Cairo::Context> &cr, int width, int height);

    void pen(std::vector<ink::stroke_model::Result> &stroke);

    void pencil(std::vector<ink::stroke_model::Result> &stroke);

    void solid_brush(std::vector<ink::stroke_model::Result> &stroke);

    void clear_pencil_data();

    void step_forward();

    void step_backward();

    void play_next();

    void fill_area(int x, int y);

    unsigned char *pixel(int x, int y);

    unsigned char * pixel2(int x, int y);

    void onion_skin();
};
#endif //DRAWINGS_H