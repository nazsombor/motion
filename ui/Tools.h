//
// Created by abraham on 8/30/24.
//

#ifndef TOOLS_H
#define TOOLS_H
#include <gtkmm.h>

#include "ColorList.h"
#include "ColorPicker.h"
#include "CurrentColor.h"


class Tools : public Gtk::Box{
public:
    enum Type {PEN, PENCIL, SOLID_BRUSH, TEXTURED_BRUSH, COLOR_PICKER_TOOL};
    Type type = PEN;
    Gtk::Box tools_box;
    Gtk::Button pen;
    Gtk::Button pencil;
    Gtk::Button solid_brush;
    Gtk::Button textured_brush;
    ColorList   color_list;
    CurrentColor current_color;
    ColorPicker color_picker;
    bool solid_brush_selected;

    Tools();

    void select_pen();

    void select_pencil();

    void select_solid_brush();

    void select_textured_brush();
};



#endif //TOOLS_H
