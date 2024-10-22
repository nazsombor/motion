//
// Created by abraham on 10/6/24.
//

#include "History.h"

Action::Action(bool is_pen_or_pencil, bool new_frame_added, int frame_index, int layer_index,
    Cairo::RefPtr<Cairo::ImageSurface> &surface): is_pen_or_pencil(is_pen_or_pencil), new_frame_added(new_frame_added), frame_index(frame_index), layer_index(layer_index), surface(surface) {
}

void History::append_drawing(Cairo::RefPtr<Cairo::ImageSurface> &surface, bool is_pen_or_pencil,
    bool new_frame_added, int frame_index, int layer_index) {
    actions.emplace(is_pen_or_pencil, new_frame_added, frame_index, layer_index, surface);
}
