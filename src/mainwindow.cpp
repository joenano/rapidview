#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTabBar>
#include <QElapsedTimer>
#include <QDebug>

#include "mainwindow.h"
#include "ui/ui_mainwindow.h"

#include "rapidjson/pointer.h"

using namespace rapidjson;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->input_ptr->setFrame(false);

    open_tabs = new Tabs;

    current_json_view = nullptr;

    connect(ui->btn_open_json, &QPushButton::clicked, this, &MainWindow::open_json);
    connect(ui->btn_topbar_open, &QPushButton::clicked, this, &MainWindow::open_json);
    connect(ui->tabs_main->tabBar(), &QTabBar::tabCloseRequested, this, &MainWindow::close_tab);
    connect(ui->tabs_main->tabBar(), &QTabBar::currentChanged, this, [this](int index) {
        if(index && open_tabs->size())
            current_json_view = open_tabs->at_index(index)->tab;
    });
    connect(ui->input_ptr, &QLineEdit::textChanged, this, &MainWindow::search);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear_subtree_model(const QString &key)
{
    for(auto i = subtree_models[key].begin(); i != subtree_models[key].end(); i++)
        delete *i;

    subtree_models.remove(key);
}

void MainWindow::close_tab(const int index)
{
    QWidget *widget = ui->tabs_main->widget(index);
    ui->tabs_main->removeTab(index);
    delete widget;

    if(open_tabs->size()) {
        clear_subtree_model(open_tabs->at_index(index)->tab->json->filename);
        open_tabs->remove(index);
    }

    if(open_tabs->size())
        current_json_view = open_tabs->at_index(ui->tabs_main->currentIndex())->tab;
}

void MainWindow::display_msg_box(const QString msg, const QString title)
{
    QMessageBox msg_box;
    msg_box.setText(msg);
    msg_box.setWindowTitle(title);
    msg_box.setIcon(QMessageBox::Critical);
    msg_box.addButton(QMessageBox::Ok);
    msg_box.exec();
}

QByteArray MainWindow::make_pointer(const QByteArrayList &keys)
{
    QByteArray ptr;

    for(auto i = keys.crbegin() + 1; i != keys.crend(); i++) {
        ptr.append('/');
        ptr.append(*i);
    }

    return ptr;
}

void MainWindow::open_json()
{
    static QString path = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0);
    const QString filename = QFileDialog::getOpenFileName(this, tr("Open JSON"), path, tr("JSON Files (*.json)"));

    path = QFileInfo(filename).path();

    if(!filename.isEmpty()) {
        QElapsedTimer t;
        t.start();

        QByteArray file = read_file(filename);

        Document *doc = new Document;
        doc->Parse(file.data());

        if(doc && doc->HasParseError()) {
            delete doc;
            display_msg_box("Failed to parse JSON from: " + filename, "Invalid JSON");
        }
        else {
            if(ui->tabs_main->widget(0) && ui->tabs_main->widget(0)->objectName() == "tab_welcome")
                close_tab(0);

            JsonFile *json_file = new JsonFile(filename, doc);
            JsonView *json_view = new JsonView(json_file);

            open_tabs->append(json_view);
            current_json_view = json_view;

            int index = ui->tabs_main->addTab(json_view, filename.mid(path.length() + 1));
            ui->tabs_main->setCurrentIndex(index);

            connect(json_view->view, &QAbstractItemView::clicked, this, &MainWindow::view_clicked);
        }
        qDebug() << t.elapsed();
    }
}

QByteArray MainWindow::read_file(const QString &filename)
{
    QFile file(filename);
    QByteArray buffer;

    if(file.open(QFile::ReadOnly)) {
        buffer = file.readAll();
        file.close();
    }

    return buffer;
}

void MainWindow::search(const QString &text)
{
    if(!current_json_view || !text.size() || text[0] != '/') return;

    QByteArray ptr = text.toUtf8();

    if(subtree_models[current_json_view->json->filename].contains(ptr)) {
        ui->view_object->setModel(subtree_models[current_json_view->json->filename][ptr]);
        ui->view_object->expand(subtree_models[current_json_view->json->filename][ptr]->index(0, 0));
    }
    else {
        auto model = subtree(current_json_view, ptr, ptr.split('/').last());

        if(model) {
            ui->view_object->setModel(model);
            ui->view_object->expand(model->index(0, 0));
        }
    }
}

QStandardItemModel *MainWindow::subtree(const JsonView *tab, const QByteArray &ptr, const QString &key)
{
    if(subtree_models[tab->json->filename].contains(ptr))
        return subtree_models[tab->json->filename][ptr];

    QStandardItemModel *model = nullptr;

    const Value *obj = Pointer(ptr).Get(*tab->json->doc);

    if(obj && (obj->IsObject() || obj->IsArray())) {
        model = new QStandardItemModel;
        QStandardItem *json = new QStandardItem(key);
        model->appendRow(json);
        current_json_view->recurse_json(json, *obj);
    }

    if(model)
        subtree_models[tab->json->filename][ptr] = model;

    return model;
}

void MainWindow::view_clicked(const QModelIndex &index)
{
    QByteArrayList keys = {index.data().toByteArray()};

    auto parent = index.parent();

    while(parent.isValid()) {
        keys << parent.data().toByteArray();
        parent = parent.parent();
    }

    QByteArray ptr = make_pointer(keys);

    auto model = subtree(current_json_view, ptr, keys[0]);

    if(!model) {
        keys.pop_front();
        ptr = make_pointer(keys);
        model = subtree(current_json_view, ptr, keys[0]);
    }

    if(model && !ptr.isEmpty()) {
        ui->input_ptr->setText(ptr);
        ui->view_object->setModel(model);
        ui->view_object->expand(model->index(0, 0));
    }
}
