//
// Created by abraham on 9/11/24.
//

#ifndef TIMELINE_H
#define TIMELINE_H
#include <gtkmm/box.h>

#include "../Drawings.h"


class Frame : public Gtk::DrawingArea {
    public:
};


class MoveLayer : public Gtk::DrawingArea {
    public:
    MoveLayer();

    void on_draw(const Glib::RefPtr<Cairo::Context>& ctx, int width, int height);
};


class Layer : public Gtk::Box {
    public:
    std::vector<Frame*> frames;
    MoveLayer ml;

    Layer();
};

class LayerHeader : public Gtk::Viewport {
public:
    int width, height;
    LayerHeader(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v);

    Gtk::SizeRequestMode get_request_mode_vfunc() const override;

    void measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural, int &minimum_baseline, int &natural_baseline) const override;

};

class LayerContent : public Gtk::Viewport {
public:
    int width, height;
    LayerContent(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v);

    Gtk::SizeRequestMode get_request_mode_vfunc() const override;

    void measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural, int &minimum_baseline, int &natural_baseline) const override;
};



class Timeline : public Gtk::Box {
    Gtk::Box button_container, layer_container, content_container;
    Gtk::Button add_layer_button, add_inbetween_button;
    Glib::RefPtr<Gtk::Adjustment> layer_v_adjustment, layer_header_h_adjustment, layer_content_h_adjustment;
    LayerHeader header;
    LayerContent content;
    Gtk::Scrollbar hs, vs;

public:
    std::vector<Layer*> layers;
    Timeline();

    void resize(int width, int height);
};



#endif //TIMELINE_H
