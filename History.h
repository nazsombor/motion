//
// Created by abraham on 10/6/24.
//

#ifndef HISTORY_H
#define HISTORY_H
#include <stack>
#include "ui/Tools.h"

class Action {
public:
    bool is_pen_or_pencil;
    bool new_frame_added;
    int frame_index, layer_index;
    Cairo::RefPtr<Cairo::ImageSurface> surface;

    Action(bool is_pen_or_pencil, bool new_frame_added, int frame_index, int layer_index, Cairo::RefPtr<Cairo::ImageSurface> &surface);
};

class History {
public:
    Tools *tools;
    std::stack<Action> actions;

    void append_drawing(Cairo::RefPtr<Cairo::ImageSurface> & ref, bool is_pen_or_pencil, bool new_frame_added, int frame_index, int layer_index);
};



#endif //HISTORY_H
