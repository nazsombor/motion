//
// Created by abraham on 8/30/24.
//

#include "Tools.h"

#include <iostream>

Tools::Tools() : current_color(&color_list), color_picker(&current_color){
    append(tools_container);
    append(color_picker);
    append(current_color);
    append(color_list);
    set_orientation(Gtk::Orientation::VERTICAL);
    tools_container.set_orientation(Gtk::Orientation::HORIZONTAL);
    tools_container.append(pen);
    tools_container.append(pencil);
    tools_container.append(solid_brush);
    tools_container.append(textured_brush);

    pen.set_label("pen");
    pencil.set_label("pencil");
    solid_brush.set_label("solid brush");
    textured_brush.set_label("textured brush");

    pen.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_pen));
    pencil.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_pencil));
    solid_brush.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_solid_brush));
    textured_brush.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_textured_brush));


    switch(type) {
        case PEN:
            select_pen();
            break;
        case PENCIL:
            select_pencil();
            break;
        case SOLID_BRUSH:
            select_solid_brush();
            break;
        case TEXTURED_BRUSH:
            select_textured_brush();
            break;
    }

    append(file_operation);

}

void Tools::select_pen() {
    type = PEN;
    pen.add_css_class("selected-tool");
    pencil.remove_css_class("selected-tool");
    solid_brush.remove_css_class("selected-tool");
    textured_brush.remove_css_class("selected-tool");
    solid_brush_selected = false;
}

void Tools::select_pencil() {
    type = PENCIL;
    pen.remove_css_class("selected-tool");
    pencil.add_css_class("selected-tool");
    solid_brush.remove_css_class("selected-tool");
    textured_brush.remove_css_class("selected-tool");
    solid_brush_selected = false;
}

void Tools::select_solid_brush() {
    type = SOLID_BRUSH;
    pen.remove_css_class("selected-tool");
    pencil.remove_css_class("selected-tool");
    solid_brush.add_css_class("selected-tool");
    textured_brush.remove_css_class("selected-tool");
    solid_brush_selected = true;
}

void Tools::select_textured_brush() {
    type = TEXTURED_BRUSH;
    pen.remove_css_class("selected-tool");
    pencil.remove_css_class("selected-tool");
    solid_brush.remove_css_class("selected-tool");
    textured_brush.add_css_class("selected-tool");
    solid_brush_selected = false;
}
