#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "opentabs.h"
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
    JsonTab *in_focus;
    OpenTabs *open_tabs;
    Settings *settings;
    Ui::MainWindow *ui;

    QHash<QByteArray, QStandardItemModel*> subtree_models;

    void close_tab(const int index);
    void display_msg_box(const QString msg, const QString title="Error");
    void load_settings();
    QByteArray make_pointer(const QByteArrayList &keys);
    void open_json();
    QByteArray read_file(const QString &filename);
    void search(const QString &text);
    void tweak_ui();
    QStandardItemModel *subtree(const JsonTab *tab, const QByteArray &ptr, const QByteArray &key);
    void view_clicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
