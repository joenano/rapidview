#ifndef JSONVIEW_H
#define JSONVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTreeView>

#include "rapidjson/document.h"

struct QueueItem {
    QStandardItem *node;
    const rapidjson::Value *value;
    QString key;

    QueueItem(
        QStandardItem *node,
        const rapidjson::Value *value,
        QString key
        ): node(node), value(value), key(key) {}
};

struct JsonFile {
    const QString filename;
    rapidjson::Document *doc;

    JsonFile(const QString filename, rapidjson::Document *doc): filename(filename), doc(doc) {}
};

namespace Ui {
class JsonTab;
}

class JsonView : public QWidget
{
    Q_OBJECT

    // const QPixmap type_icons[7] {
    //     QPixmap::fromImage(QImage(":/gfx/icons/types/null.svg")).scaled(7, 7),
    //     QPixmap::fromImage(QImage(":/gfx/icons/types/bool.svg")).scaled(7, 7),
    //     QPixmap::fromImage(QImage(":/gfx/icons/types/bool.svg")).scaled(7, 7),
    //     QPixmap::fromImage(QImage(":/gfx/icons/types/object.svg")),
    //     QPixmap::fromImage(QImage(":/gfx/icons/types/array.svg")),
    //     QPixmap::fromImage(QImage(":/gfx/icons/types/string.svg")).scaled(7, 7),
    //     QPixmap::fromImage(QImage(":/gfx/icons/types/number.svg")).scaled(7, 7)
    // };

    const QPixmap type_icons[2] {
        QPixmap::fromImage(QImage(":/gfx/icons/types/object.svg")),
        QPixmap::fromImage(QImage(":/gfx/icons/types/array.svg")),
    };

public:
    JsonFile *json;
    QTreeView *view;

private:
    Ui::JsonTab *ui;
    QStandardItemModel *model;

public:
    explicit JsonView(JsonFile *json, QWidget *parent = nullptr);
    ~JsonView();

    void recurse_json(QStandardItem *parent, const rapidjson::Value &value);

private:
    void load_model_json(const rapidjson::Value &value);
    void parse_value(
        QStandardItem *parent,
        const QString &key,
        const rapidjson::Value &value);

    void set_item_data(
        QStandardItem *item,
        const QString &key,
        const rapidjson::Value &value,
        const rapidjson::Type value_type
    );

};

#endif // JSONVIEW_H
