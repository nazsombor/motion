//
// Created by abraham on 9/11/24.
//

#ifndef TIMELINE_H
#define TIMELINE_H
#include <gtkmm/box.h>

#include "../Drawings.h"


class Frame : public Gtk::DrawingArea {
    public:
};


class MoveLayer : public Gtk::DrawingArea {
    public:
    MoveLayer();

    void on_draw(const Glib::RefPtr<Cairo::Context>& ctx, int width, int height);
};


class Layer : public Gtk::Box {
    public:
    std::vector<Frame*> frames;
    MoveLayer ml;

    Layer();
};



class Timeline : public Gtk::Box {
public:
    std::vector<Layer*> layers;
    Timeline();
};



#endif //TIMELINE_H
