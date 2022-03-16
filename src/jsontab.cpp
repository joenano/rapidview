#include "jsontab.h"
#include "ui_jsontab.h"

#include <QDebug>


JsonTab::JsonTab(JsonFile *json, QWidget *parent) : QWidget(parent), ui(new Ui::JsonTab)
{
    ui->setupUi(this);

    load_model_json(*json->doc);
}

JsonTab::~JsonTab()
{
    delete model_json;
    delete ui;
}

void JsonTab::load_model_json(const rapidjson::Value &v)
{
    model_json = new QStandardItemModel;

    QStandardItem *json = new QStandardItem("JSON");
    model_json->appendRow(json);

    recurse_json(v, json);

    ui->tree_json->setModel(model_json);
}

void JsonTab::recurse_json(const rapidjson::Value &v, QStandardItem *parent)
{
    static auto is_iterable = [](rapidjson::Type type) {
        return type == rapidjson::kObjectType || type == rapidjson::kArrayType;
    };

    if(v.IsObject()) {
        parent->setIcon(type_icons[rapidjson::kObjectType]);

        for(const auto &element: v.GetObject()) {
            QString key = element.name.GetString();
            QStandardItem *child = new QStandardItem;
            parent->appendRow(child);

            auto value_type = element.value.GetType();

            if(is_iterable(value_type)) {
                child->setData(key, Qt::DisplayRole);
                recurse_json(element.value, child);
            }
            else {
                set_item_text(child, key, element.value, value_type);
            }
        }
    }
    else if(v.IsArray()) {
        parent->setIcon(type_icons[rapidjson::kArrayType]);

        unsigned int i = 0;
        for(const auto &element: v.GetArray()) {
            QString key = QString::number(i++);
            QStandardItem *child = new QStandardItem;
            parent->appendRow(child);

            auto value_type = element.GetType();

            if(is_iterable(value_type)) {
                child->setData(key, Qt::DisplayRole);
                recurse_json(element, child);
            }
            else {
                set_item_text(child, key, element, value_type);
            }
        }
    }
}

void JsonTab::set_item_text(QStandardItem *item, const QString &key,
                            const rapidjson::Value &v, const rapidjson::Type value_type)
{
    if(value_type == rapidjson::kStringType) {
        QByteArray value = v.GetString();
        QString data = QString("%1: \"%2\"").arg(key, value);
        item->setData(data, Qt::DisplayRole);
    }
    else if(value_type == rapidjson::kNumberType) {
        QString value = string_from_number(v);
        QString data = QString("%1: %2").arg(key, value);
        item->setData(data, Qt::DisplayRole);
    }
    else if(value_type == rapidjson::kNullType) {
        QString data = QString("%1: null").arg(key);
        item->setData(data, Qt::DisplayRole);
    }
    else if(value_type == rapidjson::kFalseType || value_type == rapidjson::kTrueType) {
        bool value = v.GetBool();
        QString data = QString("%1: %2").arg(key, value? "true" : "false");
        item->setData(data, Qt::DisplayRole);
    }
}

QString JsonTab::string_from_number(const rapidjson::Value &v)
{
    QString number;

    if(v.IsInt64()) {

    }

    return number;
}
