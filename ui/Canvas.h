//
// Created by abraham on 8/14/24.
//

#ifndef CANVAS_H
#define CANVAS_H
#include <gtkmm.h>


class Canvas : public Gtk::DrawingArea{
    int canvas_width = 1920, canvas_height = 1080;
public:
    Canvas();
    void resize(int width, int height);

    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

};



#endif //CANVAS_H
