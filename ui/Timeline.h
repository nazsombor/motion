//
// Created by abraham on 9/11/24.
//

#ifndef TIMELINE_H
#define TIMELINE_H
#include <gtkmm/box.h>

#include "Placeholder.h"
#include "../Drawings.h"


class Frame : public Gtk::DrawingArea {
    public:
};


class MoveLayer : public Gtk::DrawingArea {
    public:
    MoveLayer();

    void on_draw(const Glib::RefPtr<Cairo::Context>& ctx, int width, int height);
};


class Layer {
    public:
    std::vector<Frame*> frames;
    Gtk::DrawingArea background;
    MoveLayer ml;

    Layer();

    void on_draw(const Glib::RefPtr<Cairo::Context>& ctx, int width, int height);
};

class LayerHeader : public Gtk::Viewport {
public:
    Gtk::Box container;
    Placeholder top_margin;
    int width, height;
    LayerHeader(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v);

    Gtk::SizeRequestMode get_request_mode_vfunc() const override;

    void measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural, int &minimum_baseline, int &natural_baseline) const override;

};

class LayerContent : public Gtk::Viewport {
    Glib::RefPtr<Gtk::GestureStylus> gs;
public:
    Gtk::Box container;
    int width, height;
    LayerContent(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v);

    Gtk::SizeRequestMode get_request_mode_vfunc() const override;

    void measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural, int &minimum_baseline, int &natural_baseline) const override;

    void on_stylus_down(double x, double y);

    void on_stylus_motion(double x, double y);

    void on_stylus_up(double x, double y);

};

class TimelineNumbers : public Gtk::Viewport {
    Gtk::DrawingArea frames;
    int frame_index = 0;
public:

    int width;

    TimelineNumbers(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v);

    void on_draw(const Glib::RefPtr<Cairo::Context>& ctx, int width, int height);

    Gtk::SizeRequestMode get_request_mode_vfunc() const override;

    void measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural, int &minimum_baseline, int &natural_baseline) const override;

    void set_frame_index(int index);

};



class Timeline : public Gtk::Box {
    // UI
    Gtk::Box button_container, layer_container, content_container;
    Gtk::Button add_layer_button, add_inbetween_button;
    Glib::RefPtr<Gtk::Adjustment> layer_v_adjustment, layer_header_h_adjustment, layer_content_h_adjustment, not_in_use;
    TimelineNumbers timeline_numbers;
    LayerHeader header;
    LayerContent content;
    Gtk::Scrollbar hs, vs;

    // State
    int frame_index = 0;
    int layer_index = 0;
    int end_frame_index = 0;
    int end_layer_index = 0;

public:
    std::vector<Layer*> layers;
    Timeline();

    void resize(int width, int height);

    void on_click();

    void append_new_layer();
};



#endif //TIMELINE_H
