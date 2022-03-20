#include "jsontab.h"
#include "ui_jsontab.h"

using namespace rapidjson;


JsonTab::JsonTab(JsonFile *json, Settings *settings, QWidget *parent)
    : QWidget(parent), ui(new Ui::JsonTab), settings(settings)
{
    ui->setupUi(this);
    load_model_json(*json->doc);

    // expand the root node
    ui->tree_json->expand(model_json->index(0, 0));
}

JsonTab::~JsonTab()
{
    delete ui;
    delete model_json;
}

void JsonTab::load_model_json(const Value &v)
{
    model_json = new QStandardItemModel;

    QStandardItem *json = new QStandardItem("JSON");
    model_json->appendRow(json);

    recurse_json(v, json);

    ui->tree_json->setModel(model_json);
}

void JsonTab::recurse_json(const Value &v, QStandardItem *parent)
{
    if(v.IsObject()) {
        parent->setIcon(type_icons[kObjectType]);

        for(const auto &element: v.GetObject()) {
            QString key = element.name.GetString();
            QStandardItem *child = new QStandardItem;
            parent->appendRow(child);

            auto value_type = element.value.GetType();

            if(value_type == kObjectType || value_type == kArrayType) {
                child->setData(key, Qt::DisplayRole);
                recurse_json(element.value, child);
            }
            else {
                set_item_text(child, key, element.value, value_type);
            }
        }
    }
    else if(v.IsArray()) {
        parent->setIcon(type_icons[kArrayType]);

        unsigned int i = 0;
        for(const auto &element: v.GetArray()) {
            QString key = QString::number(i++);
            QStandardItem *child = new QStandardItem;
            parent->appendRow(child);

            auto value_type = element.GetType();

            if(value_type == kObjectType || value_type == kArrayType) {
                child->setData(key, Qt::DisplayRole);
                recurse_json(element, child);
            }
            else {
                set_item_text(child, key, element, value_type);
            }
        }
    }
}

void JsonTab::set_item_text(QStandardItem *item, const QString &key, const Value &v, const Type value_type)
{
    const auto set_item_data = [&](QStandardItem *item, QString data, Type type) {
        if(settings->colour_code_types)
            item->setIcon(type_icons[type]);
        item->setData(data, Qt::DisplayRole);
    };

    if(value_type == kStringType) {
        QByteArray value = v.GetString();
        QString data = QString("%1: \"%2\"").arg(key, value);
        set_item_data(item, data, value_type);
    }
    else if(value_type == kNumberType) {
        QString value = string_from_number(v);
        QString data = QString("%1: %2").arg(key, value);
        set_item_data(item, data, value_type);
    }
    else if(value_type == kNullType) {
        QString data = QString("%1: null").arg(key);
        set_item_data(item, data, value_type);
    }
    else if(value_type == kFalseType || value_type == kTrueType) {
        bool value = v.GetBool();
        QString data = QString("%1: %2").arg(key, value? "true" : "false");
        set_item_data(item, data, value_type);
    }
}

QString JsonTab::string_from_number(const Value &v)
{
    QString number;

    if(v.IsDouble())
        number = QString::number(v.GetDouble());
    else if(v.IsInt64())
        number = QString::number(v.GetInt64());
    else if(v.IsUint64())
        number = QString::number(v.GetUint64());

    return number;
}
