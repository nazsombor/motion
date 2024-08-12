//
// Created by abraham on 8/12/24.
//

#include "Cairomotion.h"

Cairomotion::Cairomotion(): p1(60, 60, Placeholder::RED), p2(60, 60, Placeholder::GREEN),
                            p3(60, 60, Placeholder::YELLOW),
                            pb2(&p2, &p3, PopupBar::LEFT),
                            pb1(&pb2, &p1, PopupBar::BOTTOM) {
    set_default_size(1300, 900);
    set_child(pb1);
}
