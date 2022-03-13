#include "jsontab.h"
#include "ui_jsontab.h"

JsonTab::JsonTab(QWidget *parent, rapidjson::Document *doc) : QWidget(parent), ui(new Ui::JsonTab)
{
    ui->setupUi(this);
}

JsonTab::~JsonTab()
{
    delete ui;
}
