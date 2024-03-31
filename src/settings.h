#ifndef SETTINGS_H
#define SETTINGS_H



struct Settings {
    bool colour_code_types;

    Settings(rapidjson::Document *doc = nullptr);
};

#endif // SETTINGS_H
