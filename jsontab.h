#ifndef JSONTAB_H
#define JSONTAB_H

#include <QWidget>

#include "rapidjson/document.h"


namespace Ui {
class JsonTab;
}

class JsonTab : public QWidget
{
    Q_OBJECT

public:
    explicit JsonTab(QWidget *parent = nullptr, rapidjson::Document *doc = nullptr);
    ~JsonTab();

private:
    Ui::JsonTab *ui;
};

#endif // JSONTAB_H
