#include <iostream>
#include <gtkmm/application.h>

#include "MotionApp.h"

int main(int argc, char **argv) {
    auto app = Gtk::Application::create("hu.azsn.cairomotion");
    return app->make_window_and_run<MotionApp>(argc, argv);
}
