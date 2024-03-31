#include "jsonview.h"

#include "ui/ui_jsontab.h"

#include <QElapsedTimer>

#include <QDebug>

using namespace rapidjson;

JsonView::JsonView(JsonFile *json, QWidget *parent):
    QWidget(parent), json(json), ui(new Ui::JsonTab)
{
    ui->setupUi(this);

    view = ui->tree_json;

    load_model_json(*json->doc);

    ui->tree_json->expand(model->index(0, 0));
}

JsonView::~JsonView()
{
    delete ui;
    delete model;
}

void JsonView::load_model_json(const Value &value)
{
    model = new QStandardItemModel;

    QStandardItem *json = new QStandardItem("JSON");

    model->appendRow(json);

    QElapsedTimer t;

    t.start();

    recurse_json(json, value);

    qDebug() << "Json Recursed" << t.elapsed();

    ui->tree_json->setModel(model);

}

void JsonView::parse_value(QStandardItem *parent, const QString &key, const Value &value)
{
    QStandardItem *child = new QStandardItem;
    parent->appendRow(child);

    auto valueType = value.GetType();

    switch (valueType) {
        case rapidjson::kObjectType:
        case rapidjson::kArrayType:
            child->setData(key, Qt::DisplayRole);
            recurse_json(child, value);
            break;
        default:
            set_item_data(child, key, value, valueType);
            break;
    }
}

void JsonView::recurse_json(QStandardItem *parent, const Value &value)
{
    const bool isObject = value.IsObject();
    parent->setIcon(type_icons[isObject ? 0 : 1]);

    if (isObject) {
        for (const auto& member : value.GetObject()) {
            QString key = QString::fromUtf8(member.name.GetString());
            parse_value(parent, key, member.value);
        }
    } else {
        for (rapidjson::SizeType i = 0; i < value.Size(); ++i) {
            QString key = QString::number(i);
            parse_value(parent, key, value[i]);
        }
    }
}

void JsonView::set_item_data(QStandardItem *item, const QString &key, const Value &value, const Type value_type)
{
    const auto set_data = [this, item, &key](const QString& data) {
        QString displayData = QString("%1: %2").arg(key, data);
        item->setData(displayData, Qt::DisplayRole);
    };

    switch (value_type) {
        case rapidjson::kNullType:
            set_data("null");
            break;
        case rapidjson::kFalseType:
            set_data("false");
            break;
        case rapidjson::kTrueType:
            set_data("true");
            break;
        case rapidjson::kNumberType:
            set_data(QString::number(value.GetDouble()));
            break;
        case rapidjson::kStringType:
            set_data(QString::fromUtf8(value.GetString()));
            break;
        default:
            break;
    }
}
