//
// Created by abraham on 8/12/24.
//

#include "Cairomotion.h"

Cairomotion::Cairomotion(): p1(300, 300, Placeholder::RED), p2(300, 100, Placeholder::GREEN), pb1(&p1, &p2){

    set_default_size(1300, 900);
    set_child(pb1);
}
