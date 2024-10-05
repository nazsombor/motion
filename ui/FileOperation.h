//
// Created by abraham on 9/29/24.
//

#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <gtkmm.h>
#include <fstream>
#include <cairomm/cairomm.h>

class FrameEntity {
    public:
    int index, duration;
    Glib::RefPtr<Cairo::ImageSurface> surface, surface2;

    FrameEntity();
    FrameEntity(int index, int duration, Glib::RefPtr<Cairo::ImageSurface> &surface, Glib::RefPtr<Cairo::ImageSurface> &surface2);
};

class LayerEntity {
    public:
    int index;
    std::vector<FrameEntity> frames;

    LayerEntity(int index);
};

class ColorEntity {
    public:
    int r, g, b;
    std::string name;
    ColorEntity();
    ColorEntity(int r, int g, int b, std::string name);
};

class PNGStream {
public:
    std::fstream *file;

    PNGStream(std::fstream *file);

    Cairo::ErrorStatus write(const unsigned char *data, unsigned int length);

    Cairo::ErrorStatus read(unsigned char *data, unsigned int length);
};


class FileOperation : public Gtk::Box{
public:
    Gtk::Button open_file_button, save_file_button, save_as_button, export_button;
    Glib::RefPtr<Gtk::FileChooserNative> open_file_dialog, save_as_dialog, export_dialog;
    bool file_is_open = false;
    std::string path;
    bool start_saving = false;
    bool start_opening = false;
    bool start_export = false;
    std::string export_path;

    FileOperation();

    void on_open_file();

    void on_open_file_response(int response);

    void on_save_file();

    void on_save_as();

    void on_save_as_response(int response);

    void on_export();

    void on_export_response(int response);

    void save(const std::vector<LayerEntity> &layers, const std::vector<ColorEntity> &colors);

    void open(std::vector<LayerEntity> &layers, std::vector<ColorEntity> &colors);
};



#endif //FILEOPERATION_H
