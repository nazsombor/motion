//
// Created by abraham on 10/6/24.
//

#ifndef HISTORY_H
#define HISTORY_H
#include <vector>

class Action {

};

class History {
public:
    std::vector<Action> actions;

    void append_drawing();
};



#endif //HISTORY_H
