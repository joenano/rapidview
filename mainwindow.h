#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "jsontab.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::vector<rapidjson::Document*> documents;

    void close_tab(const int index);
    void open_json();
    void msg_box(const QString msg, const QString title="Error");
    QByteArray read_file(const QString filename);
    void tweak_ui();

};
#endif // MAINWINDOW_H
