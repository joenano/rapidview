#ifndef JSONTAB_H
#define JSONTAB_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTreeView>

#include "settings.h"

struct JsonFile {
    const QString filename;
    rapidjson::Document *doc;

    JsonFile(const QString filename, rapidjson::Document *doc): filename(filename), doc(doc) {}
};

namespace Ui {
class JsonTab;
}

class JsonTab : public QWidget
{
    Q_OBJECT

    const QPixmap type_icons[7] {
        QPixmap::fromImage(QImage(":/gfx/icons/types/null.svg")).scaled(7, 7),
        QPixmap::fromImage(QImage(":/gfx/icons/types/bool.svg")).scaled(7, 7),
        QPixmap::fromImage(QImage(":/gfx/icons/types/bool.svg")).scaled(7, 7),
        QPixmap::fromImage(QImage(":/gfx/icons/types/object.svg")),
        QPixmap::fromImage(QImage(":/gfx/icons/types/array.svg")),
        QPixmap::fromImage(QImage(":/gfx/icons/types/string.svg")).scaled(7, 7),
        QPixmap::fromImage(QImage(":/gfx/icons/types/number.svg")).scaled(7, 7)
    };

public:
    JsonFile *json;
    QTreeView *view;

private:
    QStandardItemModel *model;
    Settings *settings;
    Ui::JsonTab *ui;

public:
    explicit JsonTab(JsonFile *json, Settings *settings, QWidget *parent = nullptr);
    ~JsonTab();

    void recurse_json(QStandardItem *parent, const rapidjson::Value &value);

private:
    void load_model_json(const rapidjson::Value &value);
    void parse_value(QStandardItem *parent, const QString &key, const rapidjson::Value &value);
    void set_node_data(QStandardItem *node, const QString &key,
                       const rapidjson::Value &value, const rapidjson::Type value_type);
    QString string_from_number(const rapidjson::Value &value);
};

#endif // JSONTAB_H
