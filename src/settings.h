#ifndef SETTINGS_H
#define SETTINGS_H

#include "rapidjson/document.h"

struct Settings {
    bool colour_code_types;

    Settings(rapidjson::Document *doc = nullptr);
};

#endif // SETTINGS_H
