//
// Created by abraham on 8/14/24.
//

#ifndef CANVAS_H
#define CANVAS_H
#include <gtkmm.h>

#include "../thirdparty/ink-stroke-modeler/ink_stroke_modeler/stroke_modeler.h"
#include "../thirdparty/ink-stroke-modeler/ink_stroke_modeler/types.h"
#include "../thirdparty/ink-stroke-modeler/ink_stroke_modeler/params.h"

#include "../Drawings.h"

class Canvas : public Gtk::DrawingArea{
    int canvas_width = 1920, canvas_height = 1080;

    ink::stroke_model::StrokeModelParams params;
    ink::stroke_model::StrokeModeler modeler;
    std::chrono::milliseconds time;
    std::vector<ink::stroke_model::Result> smoothed_stroke;
    bool is_stylus_down = false;

public:
    Drawings * drawings;
    Glib::RefPtr<Gtk::GestureStylus> *gs;
    bool stylus_up_is_not_primary_button;

    Canvas();
    void resize(int width, int height);

    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

    void on_stylus_down(double x, double y);

    void on_stylus_motion(gdouble x, gdouble y);

    void on_stylus_up(double x, double y);

    void setup_gesture_stylus(Glib::RefPtr<Gtk::GestureStylus> &gs);

};



#endif //CANVAS_H
