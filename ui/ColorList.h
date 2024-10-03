//
// Created by abraham on 9/5/24.
//

#ifndef COLORLIST_H
#define COLORLIST_H
#include <gtkmm.h>

#include "ScrolledWidget.h"

class ColorList;

class ColorListItem : public Gtk::Box {
    public:
    Gtk::DrawingArea color;
    Gtk::Label text;
    int r, g, b;
    Glib::RefPtr<Gtk::GestureClick> click;
    ColorList *list;


    ColorListItem(int r, int g, int b, std::string string, ColorList *color_list);

    void on_draw(const Cairo::RefPtr<Cairo::Context>& context, int width, int height);

    void on_click(int count, double x, double y);
};



class ColorList : public ScrolledWidget{
    Gtk::Box list;

public:
    std::vector<ColorListItem*> items;
    double r, g, b;
    bool update_color_picker = false;

    ColorList();
    ~ColorList();

    void add_color(int i, int g, int b, std::string ustring);

    void deselect_all();
};



#endif //COLORLIST_H
