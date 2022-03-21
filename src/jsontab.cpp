#include "jsontab.h"
#include "ui_jsontab.h"

using namespace rapidjson;


JsonTab::JsonTab(JsonFile *json, Settings *settings, QWidget *parent)
    : QWidget(parent), ui(new Ui::JsonTab), settings(settings)
{
    ui->setupUi(this);

    // create tree model from json document
    load_model_json(*json->doc);

    // expand root node of tree
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

    // create tree root node
    QStandardItem *json = new QStandardItem("JSON");
    model_json->appendRow(json);

    // recursively traverse json and populate tree model
    recurse_json(json, v);

    // display tree model on tab widget
    ui->tree_json->setModel(model_json);
}

void JsonTab::parse_value(QStandardItem *parent, const QString &key, const Value &v)
{
    // create new tree node and add to parent
    QStandardItem *child = new QStandardItem;
    parent->appendRow(child);

    auto value_type = v.GetType();

    // if json value type is object or array - set node data to key only and recurse further
    if(value_type == kObjectType || value_type == kArrayType) {
        child->setData(key, Qt::DisplayRole);
        recurse_json(child, v);
    }
    else {
        // else set node data to key and value
        set_node_data(child, key, v, value_type);
    }
}

void JsonTab::recurse_json(QStandardItem *parent, const Value &v)
{
    // iterate object/array, get key/index and parse values

    if(v.IsObject()) {
        parent->setIcon(type_icons[kObjectType]);

        for(const auto &element: v.GetObject()) {
            QString key = element.name.GetString();
            parse_value(parent, key, element.value);
        }
    }
    else if(v.IsArray()) {
        parent->setIcon(type_icons[kArrayType]);

        unsigned int i = 0;
        for(const auto &element: v.GetArray()) {
            QString key = QString::number(i++);
            parse_value(parent, key, element);
        }
    }
}

void JsonTab::set_node_data(QStandardItem *node, const QString &key, const Value &v, const Type value_type)
{

    const auto set_data = [&](QStandardItem *node, QString data, Type type) {
        if(settings->colour_code_types)
            node->setIcon(type_icons[type]);
        node->setData(data, Qt::DisplayRole);
    };

    // tree node data - "key: value"
    QString data;

    if(value_type == kStringType) {
        QString value = v.GetString();
        data = QString("%1: \"%2\"").arg(key, value);
    }
    else if(value_type == kNumberType) {
        QString value = string_from_number(v);
        data = QString("%1: %2").arg(key, value);
    }
    else if(value_type == kNullType) {
        data = QString("%1: null").arg(key);
    }
    else if(value_type == kFalseType || value_type == kTrueType) {
        bool value = v.GetBool();
        data = QString("%1: %2").arg(key, value? "true" : "false");
    }

    set_data(node, data, value_type);
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
