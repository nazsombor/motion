//
// Created by abraham on 9/29/24.
//

#include "FileOperation.h"


FrameEntity::FrameEntity() : surface(Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080)),
                             surface2(Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080)) {
}

FrameEntity::FrameEntity(int index, int duration, Glib::RefPtr<Cairo::ImageSurface> &surface,
                         Glib::RefPtr<Cairo::ImageSurface> &surface2) : index(index), duration(duration),
                                                                        surface(surface), surface2(surface2) {
}

LayerEntity::LayerEntity(int index) : index(index) {
}

ColorEntity::ColorEntity() {
}

ColorEntity::ColorEntity(int r, int g, int b, std::string name) : r(r), g(g), b(b), name(name) {
}

PNGStream::PNGStream(std::fstream *file) {
    this->file = file;
}

Cairo::ErrorStatus PNGStream::write(const unsigned char *data, unsigned int length) {
    file->write(reinterpret_cast<const char*>(data), length);
    return Cairo::ErrorStatus::CAIRO_STATUS_SUCCESS;
}

Cairo::ErrorStatus PNGStream::read(unsigned char *data, unsigned int length) {
    file->read(reinterpret_cast<char*>(data), length);
    return Cairo::ErrorStatus::CAIRO_STATUS_SUCCESS;
}


FileOperation::FileOperation() : open_file_dialog(
                                     Gtk::FileChooserNative::create("Open", Gtk::FileChooser::Action::OPEN, "Open",
                                                                    "Cancel")),
                                 save_as_dialog(Gtk::FileChooserNative::create(
                                     "Save As", Gtk::FileChooser::Action::SAVE, "Save As", "Cancel")),
                                 export_dialog(
                                     Gtk::FileChooserNative::create("Export", Gtk::FileChooser::Action::SAVE, "Export",
                                                                    "Cancel")) {
    set_orientation(Gtk::Orientation::HORIZONTAL);

    append(open_file_button);
    append(save_file_button);
    append(save_as_button);
    append(export_button);

    open_file_button.set_label("Open");
    save_file_button.set_label("Save");
    save_as_button.set_label("Save As");
    export_button.set_label("Export");

    open_file_button.signal_clicked().connect(sigc::mem_fun(*this, &FileOperation::on_open_file));
    save_file_button.signal_clicked().connect(sigc::mem_fun(*this, &FileOperation::on_save_file));
    save_as_button.signal_clicked().connect(sigc::mem_fun(*this, &FileOperation::on_save_as));
    export_button.signal_clicked().connect(sigc::mem_fun(*this, &FileOperation::on_export));

    open_file_dialog->signal_response().connect(sigc::mem_fun(*this, &FileOperation::on_open_file_response));
    save_as_dialog->signal_response().connect(sigc::mem_fun(*this, &FileOperation::on_save_as_response));
    export_dialog->signal_response().connect(sigc::mem_fun(*this, &FileOperation::on_export_response));
}

void FileOperation::on_open_file() {
    open_file_dialog->show();
}

void FileOperation::on_open_file_response(int response) {
    if (response == Gtk::ResponseType::ACCEPT) {
        path = open_file_dialog->get_file()->get_path();
        file_is_open = true;
        start_opening = true;
    }
}

void FileOperation::on_save_file() {
    if (file_is_open) {
        start_saving = true;
    } else {
        save_as_dialog->show();
    }
}

void FileOperation::on_save_as() {
    save_as_dialog->show();
}

void FileOperation::on_save_as_response(int response) {
    if (response == Gtk::ResponseType::ACCEPT) {
        path = save_as_dialog->get_file()->get_path();
        file_is_open = true;
        start_saving = true;
    }
}

void FileOperation::on_export() {
    export_dialog->show();
}

void FileOperation::on_export_response(int response) {
    if (response == Gtk::ResponseType::ACCEPT) {
        export_path = export_dialog->get_file()->get_path();
        start_export = true;
    }
}

void FileOperation::save(const std::vector<LayerEntity> &layers, const std::vector<ColorEntity> &colors) {
    std::fstream file;
    file.open(path, std::ios::binary | std::ios::out);

    PNGStream helper(&file);

    auto layer_size = layers.size();
    file.write(reinterpret_cast<char *>(&layer_size), sizeof(layer_size));

    for (auto layer: layers) {
        auto frame_size = layer.frames.size();
        file.write(reinterpret_cast<char *>(&frame_size), sizeof(frame_size));

        for (auto frame: layer.frames) {
            file.write(reinterpret_cast<char *>(&frame.index), sizeof(frame.index));
            file.write(reinterpret_cast<char *>(&frame.duration), sizeof(frame.duration));


            // write_to_png_stream only works once per surface lifecycle, so a copy is needed
            auto s1 = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
            auto s2 = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, 1920, 1080);
            auto c1 = Cairo::Context::create(s1);
            auto c2 = Cairo::Context::create(s2);

            c1->set_source(frame.surface, 0, 0);
            c1->paint();

            c2->set_source(frame.surface2, 0, 0);
            c2->paint();

            s1->write_to_png_stream(sigc::mem_fun(helper, &PNGStream::write));
            s2->write_to_png_stream(sigc::mem_fun(helper, &PNGStream::write));

            // auto data = frame.surface->get_data();
            // for (int i = 0; i < frame.surface->get_stride() * frame.surface->get_height(); i++) {
            //     file.write(reinterpret_cast<char *>(&data[i]), sizeof(data[i]));
            // }
            //
            // data = frame.surface2->get_data();
            // for (int i = 0; i < frame.surface2->get_stride() * frame.surface2->get_height(); i++) {
            //     file.write(reinterpret_cast<char *>(&data[i]), sizeof(data[i]));
            // }
        }
    }

    auto color_size = colors.size();
    file.write(reinterpret_cast<char *>(&color_size), sizeof(color_size));

    for (auto color: colors) {
        file.write(reinterpret_cast<char *>(&color.r), sizeof(color.r));
        file.write(reinterpret_cast<char *>(&color.g), sizeof(color.g));
        file.write(reinterpret_cast<char *>(&color.b), sizeof(color.b));
        unsigned int name_size = color.name.size();
        file.write(reinterpret_cast<char *>(&name_size), sizeof(name_size));
        file.write(&color.name[0], name_size);
    }

    file.close();
}


void FileOperation::open(std::vector<LayerEntity> &layers, std::vector<ColorEntity> &colors) {
    std::fstream file;
    file.open(path, std::ios::binary | std::ios::in);

    PNGStream helper(&file);

    size_t layer_size;
    file.read(reinterpret_cast<char *>(&layer_size), sizeof(layer_size));

    for (int i = 0; i < layer_size; i++) {
        LayerEntity layer(i);

        size_t frame_size;
        file.read(reinterpret_cast<char *>(&frame_size), sizeof(frame_size));

        for (int j = 0; j < frame_size; j++) {
            FrameEntity frame{};
            file.read(reinterpret_cast<char *>(&frame.index), sizeof(frame.index));
            file.read(reinterpret_cast<char *>(&frame.duration), sizeof(frame.duration));

            frame.surface = Cairo::ImageSurface::create_from_png_stream(sigc::mem_fun(helper, &PNGStream::read));
            frame.surface2 = Cairo::ImageSurface::create_from_png_stream(sigc::mem_fun(helper, &PNGStream::read));

            // frame.surface->flush();
            // auto data = frame.surface->get_data();
            // for (int k = 0; k < frame.surface->get_stride() * frame.surface->get_height(); k++) {
            //     file.read(reinterpret_cast<char *>(&data[k]), sizeof(data[k]));
            // }
            // frame.surface->mark_dirty(0, 0, frame.surface->get_width(), frame.surface->get_height());
            //
            // frame.surface2->flush();
            // data = frame.surface2->get_data();
            // for (int k = 0; k < frame.surface2->get_stride() * frame.surface2->get_height(); k++) {
            //     file.read(reinterpret_cast<char *>(&data[k]), sizeof(data[k]));
            // }
            // frame.surface2->mark_dirty(0, 0, frame.surface2->get_width(), frame.surface2->get_height());

            layer.frames.push_back(frame);
        }

        layers.push_back(layer);
    }

    size_t color_size;
    file.read(reinterpret_cast<char *>(&color_size), sizeof(color_size));

    for (int i = 0; i < color_size; i++) {
        ColorEntity color;
        file.read(reinterpret_cast<char *>(&color.r), sizeof(color.r));
        file.read(reinterpret_cast<char *>(&color.g), sizeof(color.g));
        file.read(reinterpret_cast<char *>(&color.b), sizeof(color.b));
        unsigned int name_size;
        file.read(reinterpret_cast<char *>(&name_size), sizeof(name_size));
        color.name.resize(name_size);
        file.read(&color.name[0], name_size);
        colors.push_back(color);
    }
}
