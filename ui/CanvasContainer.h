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
        std::cout << "Hello World" << std::endl;
    }
protected:


    void size_allocate_vfunc(int width, int height, int baseline) override {
        std::cout << "..." << std::endl;

        Gtk::Widget::size_allocate_vfunc(width, height, baseline);
    }
};



#endif //CANVASCONTAINER_H
