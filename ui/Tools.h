//
// Created by abraham on 8/30/24.
//

#ifndef TOOLS_H
#define TOOLS_H
#include <gtkmm.h>

#include "ColorList.h"
#include "ColorPicker.h"
#include "CurrentColor.h"
#include "FileOperation.h"


class Tools : public Gtk::Box {
public:
    enum Type {PEN, PENCIL, SOLID_BRUSH, TEXTURED_BRUSH};
    Type type = PEN;
    Gtk::Box tools_container;
    Gtk::Button pen;
    Gtk::Button pencil;
    Gtk::Button solid_brush;
    Gtk::Button textured_brush;
    ColorList   color_list;
    CurrentColor current_color;
    ColorPicker color_picker;
    bool solid_brush_selected;
    FileOperation file_operation;

    Tools();

    void select_pen();

    void select_pencil();

    void select_solid_brush();

    void select_textured_brush();
};



#endif //TOOLS_H
