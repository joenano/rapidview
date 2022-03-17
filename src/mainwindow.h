#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "jsontab.h"
#include "settings.h"


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

    Settings *settings;

    std::vector<JsonTab::JsonFile*> open_documents;

    void close_tab(const int index);
    void display_msg_box(const QString msg, const QString title="Error");
    void open_json();
    void load_settings();
    QByteArray read_file(const QString filename);
};

#endif // MAINWINDOW_H
