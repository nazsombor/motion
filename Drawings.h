//
// Created by abraham on 8/14/24.
//

#ifndef DRAWINGS_H
#define DRAWINGS_H
#include <gtkmm.h>

#include "thirdparty/ink-stroke-modeler/ink_stroke_modeler/types.h"


class Drawings {
    Cairo::RefPtr<Cairo::ImageSurface> surface;
public:

    Drawings();

    void on_draw(const std::shared_ptr<Cairo::Context> &cr, int width, int height);

    void pen(std::vector<ink::stroke_model::Result> &stroke);

    void pencil(std::vector<ink::stroke_model::Result> &stroke);
};



#endif //DRAWINGS_H
