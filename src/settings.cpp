#include "settings.h"

Settings::Settings(rapidjson::Document *doc) : colour_code_types(false)
{
    if(doc) {
        if(doc->HasMember("colour_code_types")) {
            if((*doc)["colour_code_types"].IsBool())
                colour_code_types = (*doc)["colour_code_types"].GetBool();
        }
    }
}
