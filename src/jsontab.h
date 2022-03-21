#ifndef JSONTAB_H
#define JSONTAB_H

#include <QWidget>
#include <QStandardItemModel>

#include "settings.h"


namespace Ui {
class JsonTab;
}

class JsonTab : public QWidget
{
    Q_OBJECT

public:
    struct JsonFile {
        const QString filename;
        const rapidjson::Document *doc;

        JsonFile(const QString filename, const rapidjson::Document *doc)
            : filename(filename), doc(doc) {}
    };

    explicit JsonTab(JsonFile *json, Settings *settings, QWidget *parent = nullptr);
    ~JsonTab();

private:
    Ui::JsonTab *ui;

    Settings *settings;

    QStandardItemModel *model_json;

    const QPixmap type_icons[7] {
        QPixmap::fromImage(QImage(":/gfx/icons/types/null.svg")).scaled(7, 7),
        QPixmap::fromImage(QImage(":/gfx/icons/types/bool.svg")).scaled(7, 7),
        QPixmap::fromImage(QImage(":/gfx/icons/types/bool.svg")).scaled(7, 7),
        QPixmap::fromImage(QImage(":/gfx/icons/types/object.svg")),
        QPixmap::fromImage(QImage(":/gfx/icons/types/array.svg")),
        QPixmap::fromImage(QImage(":/gfx/icons/types/string.svg")).scaled(7, 7),
        QPixmap::fromImage(QImage(":/gfx/icons/types/number.svg")).scaled(7, 7)
    };

    void load_model_json(const rapidjson::Value &v);
    void parse_element(QStandardItem *parent, const QString &key, const rapidjson::Value &v);
    void recurse_json(QStandardItem *parent, const rapidjson::Value &v);
    void set_item_text(QStandardItem *item, const QString &key,
                       const rapidjson::Value &v, const rapidjson::Type value_type);
    QString string_from_number(const rapidjson::Value &v);
};

#endif // JSONTAB_H
