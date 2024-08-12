//
// Created by abraham on 8/12/24.
//

#ifndef CAIROMOTION_H
#define CAIROMOTION_H

#include <gtkmm.h>

#include "Placeholder.h"
#include "PopupBar.h"

class Cairomotion : public Gtk::Window {
    Placeholder p1;
    Placeholder p2;
    Placeholder p3;
    PopupBar pb2;
    PopupBar pb1;

public:
    Cairomotion();
};



#endif //CAIROMOTION_H
