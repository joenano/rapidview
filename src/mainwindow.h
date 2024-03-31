#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tabs.h"


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
    JsonView *current_json_view;
    Tabs *open_tabs;
    Ui::MainWindow *ui;

    QHash<QString, QHash<QByteArray, QStandardItemModel*>> subtree_models;

    void clear_subtree_model(const QString &key);
    void close_tab(const int index);
    void display_msg_box(const QString msg, const QString title="Error");
    QByteArray make_pointer(const QByteArrayList &keys);
    void open_json();
    QByteArray read_file(const QString &filename);
    void search(const QString &text);
    QStandardItemModel *subtree(const JsonView *tab, const QByteArray &ptr, const QString &key);
    void view_clicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
