//
// Created by abraham on 8/14/24.
//

#include "Canvas.h"

Canvas::Canvas( ) {
    set_draw_func(sigc::mem_fun(*this, &Canvas::on_draw));

    params.wobble_smoother_params.timeout = ink::stroke_model::Duration{.04};
    params.wobble_smoother_params.speed_floor = 1.31;
    params.wobble_smoother_params.speed_ceiling = 1.44;
    params.position_modeler_params.spring_mass_constant = 11.f / 32400;
    params.position_modeler_params.drag_constant = 72.f;
    params.sampling_params.min_output_rate = 500;
    params.sampling_params.end_of_stroke_stopping_distance = .001;
    params.sampling_params.end_of_stroke_max_iterations = 20;
    params.stylus_state_modeler_params.max_input_samples = 20;
    params.prediction_params = ink::stroke_model::StrokeEndPredictorParams();

    modeler.Reset(params);

}

void Canvas::resize(int width, int height) {
    auto cR = (float) width / canvas_width;

    auto w = canvas_width * cR;
    auto h = canvas_height * cR;

    if (h > height) {
        cR = (float) height / canvas_height;
        w = canvas_width * cR;
        h = canvas_height * cR;
    }

    set_content_width(w);
    set_content_height(h);
}

void Canvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height) {
    if (width == 0 || height == 0) {
        return;
    }
    cr->set_source_rgba(1, 1, 1, 1);
    cr->rectangle(0, 0, width, height);
    cr->fill();
    drawings->on_draw(cr, width, height);
}

void Canvas::on_stylus_down(double x, double y) {
    modeler.Reset();

    smoothed_stroke.clear();

    time = duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    ink::stroke_model::Input input;
    input.position = {(float) x / (get_width() / 1920.0f), (float) y / (get_height() / 1080.0f)};
    input.event_type = ink::stroke_model::Input::EventType::kDown;
    input.time = ink::stroke_model::Time{0};
    input.pressure = (*gs)->get_axis(Gdk::AxisUse::PRESSURE).value();
    input.orientation = M_PI;
    input.tilt = 0;

    modeler.Update(input, smoothed_stroke);

    drawings->clear_stroke_data();

    timeline->check_if_frame_exists();

    switch (drawings->tools->type) {
        case Tools::PEN:
            drawings->pen(smoothed_stroke);
            break;
        case Tools::PENCIL:
            drawings->pencil(smoothed_stroke);
            break;
        case Tools::SOLID_BRUSH:
            drawings->solid_brush(smoothed_stroke);
    }

    queue_draw();
}

void Canvas::on_stylus_motion(gdouble x, gdouble y) {
    auto duration = duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
);
    auto delta = duration - time;
    auto t = delta.count() / 1000.0;

    ink::stroke_model::Input input;

    input.event_type = ink::stroke_model::Input::EventType::kMove;
    input.position = {(float) x / (get_width() / 1920.0f), (float) y / (get_height() / 1080.0f)};
    input.time = ink::stroke_model::Time{t};
    input.pressure = (*gs)->get_axis(Gdk::AxisUse::PRESSURE).value();
    input.orientation = M_PI;
    input.tilt = 0;

    modeler.Update(input, smoothed_stroke);
    switch (drawings->tools->type) {
        case Tools::PEN:
            drawings->pen(smoothed_stroke);
        break;
        case Tools::PENCIL:
            drawings->pencil(smoothed_stroke);
        break;
        case Tools::SOLID_BRUSH:
            drawings->solid_brush(smoothed_stroke);
    }
    queue_draw();
}

void Canvas::on_stylus_up(double x, double y) {

    if (stylus_up_is_not_primary_button) {
        return;
    }

    auto duration = duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    auto delta = duration - time;
    auto t = delta.count() / 1000.0;

    ink::stroke_model::Input input;

    input.event_type = ink::stroke_model::Input::EventType::kUp;
    input.position = {(float) x / (get_width() / 1920.0f), (float) y / (get_height() / 1080.0f)};
    input.time = ink::stroke_model::Time{t};
    input.pressure = (*gs)->get_axis(Gdk::AxisUse::PRESSURE).value();
    input.orientation = M_PI;
    input.tilt = 0;

    modeler.Update(input, smoothed_stroke);
    switch (drawings->tools->type) {
        case Tools::PEN: {
            drawings->pen(smoothed_stroke);
            break;
        }
        case Tools::PENCIL: {
            drawings->pencil(smoothed_stroke);
            break;
        }
        case Tools::SOLID_BRUSH: {
            drawings->solid_brush(smoothed_stroke);
            break;
        }
    }
    queue_draw();
}

void Canvas::setup_gesture_stylus(Glib::RefPtr<Gtk::GestureStylus> &gs) {
    this->gs = &gs;
    gs->signal_down().connect(sigc::mem_fun(*this, &Canvas::on_stylus_down));
    gs->signal_motion().connect(sigc::mem_fun(*this, &Canvas::on_stylus_motion));
    gs->signal_up().connect(sigc::mem_fun(*this, &Canvas::on_stylus_up));

    add_controller(gs);
}
