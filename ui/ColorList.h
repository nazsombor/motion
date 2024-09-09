//
// Created by abraham on 9/5/24.
//

#ifndef COLORLIST_H
#define COLORLIST_H
#include <gtkmm.h>

#include "ScrolledWidget.h"

class ColorListItem : public Gtk::Box {
    public:
    Gtk::DrawingArea color;
    Gtk::Label text;
    int r, g, b;


    ColorListItem(int r, int g, int b, std::string string);

    void on_draw(const Cairo::RefPtr<Cairo::Context>& context, int width, int height);
};



class ColorList : public ScrolledWidget{
    Gtk::Box list;
    std::vector<ColorListItem*> items;

public:
    ColorList();
    ~ColorList();

    void add_color(int i, int g, int b, std::string ustring);
};



#endif //COLORLIST_H
