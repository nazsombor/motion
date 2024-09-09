//
// Created by abraham on 9/5/24.
//

#include "ColorList.h"

ColorListItem::ColorListItem(int r, int g, int b, std::string string) {
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
}

void ColorListItem::on_draw(const Cairo::RefPtr<Cairo::Context> &context, int width, int height) {
    context->set_source_rgb(r/ 255.0, g/ 255.0, b/ 255.0);
    context->rectangle(10, 10, width - 10, height - 10);
    context->fill();
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
    auto item = new ColorListItem(r, g, b, ustring);
    items.push_back(item);
    list.append(*item);
}
