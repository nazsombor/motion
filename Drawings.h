//
// Created by abraham on 8/14/24.
//

#ifndef DRAWINGS_H
#define DRAWINGS_H
#include <gtkmm.h>

#include "thirdparty/ink-stroke-modeler/ink_stroke_modeler/types.h"
#include "ui/Tools.h"


class Drawings {
    Cairo::RefPtr<Cairo::ImageSurface> surface, surface2;
    int stroke_index;

    int index(ink::stroke_model::Result &a, ink::stroke_model::Result &b, int stride, int j, int x, int y);

public:
    Tools * tools;

    Drawings(Tools &tools);

    void on_draw(const std::shared_ptr<Cairo::Context> &cr, int width, int height);

    void pen(std::vector<ink::stroke_model::Result> &stroke);

    void pencil(std::vector<ink::stroke_model::Result> &stroke);

    void clear_pencil_data();
};



#endif //DRAWINGS_H
