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

private:
    struct Settings {
        bool save_json_pretty;

        Settings() : save_json_pretty(false) {}
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Settings settings;

    std::vector<JsonTab::JsonFile*> open_documents;

    void close_tab(const int index);
    void open_json();
    void display_msg_box(const QString msg, const QString title="Error");
    QByteArray read_file(const QString filename);
//    void save_json(const QString filename, JsonTab::JsonFile *json);
};
#endif // MAINWINDOW_H
