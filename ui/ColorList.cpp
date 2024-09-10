//
// Created by abraham on 9/5/24.
//

#include "ColorList.h"

ColorListItem::ColorListItem(int r, int g, int b, std::string string, ColorList* list) {
    this->list = list;
    this->r = r;
    this->g = g;
    this->b = b;
    set_orientation(Gtk::Orientation::HORIZONTAL);
    color.set_content_width(50);
    color.set_content_height(50);
    color.set_draw_func(sigc::mem_fun(*this, &ColorListItem::on_draw));
    text.set_text(string);
    append(color);
    append(text);

    click = Gtk::GestureClick::create();
    click->signal_pressed().connect(sigc::mem_fun(*this, &ColorListItem::on_click));
    add_controller(click);
}

void ColorListItem::on_draw(const Cairo::RefPtr<Cairo::Context> &context, int width, int height) {
    context->set_source_rgb(r/ 255.0, g/ 255.0, b/ 255.0);
    context->rectangle(10, 10, width - 10, height - 10);
    context->fill();
}

void ColorListItem::on_click(int count, double x, double y) {
    list->deselect_all();
    get_style_context()->add_class("selected-tool");
    list->r = r / 256.0;
    list->g = g / 256.0;
    list->b = b / 256.0;
    list->update_color_picker = true;
    std::cout << "it should send the color" << std::endl;
}

ColorList::ColorList() {
    list.set_orientation(Gtk::Orientation::VERTICAL);

    set_child(list);

    resize(256, 400);
}

ColorList::~ColorList() {
    for (int i = 0; i < items.size(); i++) {
        delete items[i];
    }
}

void ColorList::add_color(int r, int g, int b, std::string ustring) {
    auto item = new ColorListItem(r, g, b, ustring, this);
    items.push_back(item);
    list.append(*item);
    this->r = r / 256.0;
    this->g = g / 256.0;
    this->b = b / 256.0;
    deselect_all();
    item->add_css_class("selected-tool");
}

void ColorList::deselect_all() {
    for (auto item : items) {
        item->remove_css_class("selected-tool");
    }
}
