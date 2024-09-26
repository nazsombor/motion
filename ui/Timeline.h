//
// Created by abraham on 9/11/24.
//

#ifndef TIMELINE_H
#define TIMELINE_H
#include <gtkmm/box.h>

#include "Placeholder.h"
#include "../Drawings.h"


class Frame {
public:
    int index, duration = 1;
    Cairo::RefPtr<Cairo::ImageSurface> surface, surface2, onion_skin;
    Frame(int index);
};


class MoveLayer : public Gtk::DrawingArea {
    Glib::RefPtr<Gtk::GestureClick> gs;

    public:
    MoveLayer();

    void on_draw(const Glib::RefPtr<Cairo::Context>& ctx, int width, int height);

    void on_click(int count, double x, double y);
};

class Timeline;

class Layer {
    public:
    int index = 0;
    std::vector<Frame*> frames;
    Gtk::DrawingArea background;
    Gtk::Box header;
    MoveLayer ml;
    Gtk::Entry label;
    Glib::RefPtr<Gtk::GestureClick> gc;
    Timeline *timeline;

    Layer(int index, Timeline *timeline);

    void on_draw(const Glib::RefPtr<Cairo::Context>& ctx, int width, int height);

    void deselect();

    void select();

    void on_click(int count, double x, double y);

    Frame *get_frame(int frame_index);

    Frame *get_previous_frame(Frame *frame);
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

    Drawings *drawings;

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
    Glib::RefPtr<Gtk::GestureClick> gc;
public:

    Timeline *timeline;
    int width;

    TimelineNumbers(Glib::RefPtr<Gtk::Adjustment> &h, Glib::RefPtr<Gtk::Adjustment> &v);

    void on_draw(const Glib::RefPtr<Cairo::Context>& ctx, int width, int height);

    Gtk::SizeRequestMode get_request_mode_vfunc() const override;

    void measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural, int &minimum_baseline, int &natural_baseline) const override;

    void set_frame_index(int index);

    void on_click(int count, double x, double y);

};



class Timeline : public Gtk::Box {
public:
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
    int next_layer_index = 0;


    Drawings *drawings;
    std::vector<Layer*> layers;
    bool request_canvas_redraw = false;

    Timeline();

    void resize(int width, int height);

    void new_layer_button_on_click();

    void append_new_layer();

    void step_forward();

    void step_backward();

    void select_layer();

    void check_if_frame_exists();

    void set_frame_index(int index);
};



#endif //TIMELINE_H
