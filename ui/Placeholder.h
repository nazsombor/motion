//
// Created by abraham on 8/12/24.
//

#ifndef PLACEHOLDER_H
#define PLACEHOLDER_H
#include <gtkmm/drawingarea.h>


class Placeholder : public Gtk::DrawingArea {
public:
    enum Color {RED, GREEN, YELLOW, BLUE, WHITE};
private:
    Color color;

public:

    Placeholder(int width, int height, Color color);

    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};



#endif //PLACEHOLDER_H
