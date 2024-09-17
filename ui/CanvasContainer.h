//
// Created by abraham on 8/13/24.
//

#ifndef CANVASCONTAINER_H
#define CANVASCONTAINER_H
#include <gtkmm/centerbox.h>
#include <iostream>

class CanvasContainer : public Gtk::CenterBox{
public:
    CanvasContainer() {
    }
protected:


    void size_allocate_vfunc(int width, int height, int baseline) override {
        Gtk::Widget::size_allocate_vfunc(width, height, baseline);
    }
};



#endif //CANVASCONTAINER_H
