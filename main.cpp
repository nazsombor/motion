#include <iostream>
#include <gtkmm/application.h>

#include "Cairomotion.h"

int main(int argc, char **argv) {
    auto app = Gtk::Application::create("hu.azsn.cairomotion");
    return app->make_window_and_run<Cairomotion>(argc, argv);
}
