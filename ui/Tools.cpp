//
// Created by abraham on 8/30/24.
//

#include "Tools.h"

#include <iostream>

Tools::Tools() {
    append(tools_box);
    append(color_picker);
    set_orientation(Gtk::Orientation::VERTICAL);
    tools_box.set_orientation(Gtk::Orientation::HORIZONTAL);
    tools_box.append(pen);
    tools_box.append(pencil);
    tools_box.append(eraser);
    tools_box.append(solid_brush);
    tools_box.append(textured_brush);

    pen.set_label("pen");
    pencil.set_label("pencil");
    eraser.set_label("eraser");
    solid_brush.set_label("solid brush");
    textured_brush.set_label("textured brush");

    pen.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_pen));
    pencil.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_pencil));
    eraser.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_eraser));
    solid_brush.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_solid_brush));
    textured_brush.signal_clicked().connect(sigc::mem_fun(*this, &Tools::select_textured_brush));

    switch(type) {
        case PEN:
            select_pen();
            break;
        case PENCIL:
            select_pencil();
            break;
        case ERASER:
            select_eraser();
            break;
        case SOLID_BRUSH:
            select_solid_brush();
            break;
        case TEXTURED_BRUSH:
            select_textured_brush();
            break;
    }
    select_pen();
}

void Tools::select_pen() {
    type = PEN;
    pen.add_css_class("selected-tool");
    pencil.remove_css_class("selected-tool");
    eraser.remove_css_class("selected-tool");
    solid_brush.remove_css_class("selected-tool");
    textured_brush.remove_css_class("selected-tool");
    solid_brush_selected = false;
}

void Tools::select_pencil() {
    type = PENCIL;
    pen.remove_css_class("selected-tool");
    pencil.add_css_class("selected-tool");
    eraser.remove_css_class("selected-tool");
    solid_brush.remove_css_class("selected-tool");
    textured_brush.remove_css_class("selected-tool");
    solid_brush_selected = false;
}

void Tools::select_eraser() {
    type = ERASER;
    pen.remove_css_class("selected-tool");
    pencil.remove_css_class("selected-tool");
    eraser.add_css_class("selected-tool");
    solid_brush.remove_css_class("selected-tool");
    textured_brush.remove_css_class("selected-tool");
    solid_brush_selected = false;
}

void Tools::select_solid_brush() {
    type = SOLID_BRUSH;
    pen.remove_css_class("selected-tool");
    pencil.remove_css_class("selected-tool");
    eraser.remove_css_class("selected-tool");
    solid_brush.add_css_class("selected-tool");
    textured_brush.remove_css_class("selected-tool");
    solid_brush_selected = true;
}

void Tools::select_textured_brush() {
    type = TEXTURED_BRUSH;
    pen.remove_css_class("selected-tool");
    pencil.remove_css_class("selected-tool");
    eraser.remove_css_class("selected-tool");
    solid_brush.remove_css_class("selected-tool");
    textured_brush.add_css_class("selected-tool");
    solid_brush_selected = false;
}
