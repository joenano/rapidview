#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTabBar>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "rapidjson/pointer.h"

using namespace rapidjson;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    load_settings();

    open_documents = new OpenTabs;

    in_focus = nullptr;

    connect(ui->btn_open_json, &QPushButton::clicked, this, &MainWindow::open_json);
    connect(ui->btn_topbar_open, &QPushButton::clicked, this, &MainWindow::open_json);
    connect(ui->tabs_main->tabBar(), &QTabBar::tabCloseRequested, this, &MainWindow::close_tab);
    connect(ui->tabs_main->tabBar(), &QTabBar::currentChanged, this, [this](int index) {
        if(index && open_documents->size())
            in_focus = open_documents->at_index(index)->tab;
    });
    connect(ui->search, &QLineEdit::textChanged, this, &MainWindow::search);
    connect(ui->btn_topbar_github, &QPushButton::clicked, this, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/joenano/rapidview"));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::close_tab(const int index)
{
    QWidget *widget = ui->tabs_main->widget(index);
    ui->tabs_main->removeTab(index);
    delete widget;

    if(open_documents->size())
        open_documents->remove(index);

    if(open_documents->size())
        in_focus = open_documents->at_index(ui->tabs_main->currentIndex())->tab;
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

void MainWindow::load_settings()
{
    QByteArray json = read_file("../../settings/settings.json");

    if(json.isEmpty()) {
        settings = new Settings();
    }
    else {
        Document *doc = new Document;
        doc->ParseInsitu(json.data());

        settings = new Settings(doc);
    }
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
    static QString path = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0);

    path = "/home/slevin/code/python/form_tools/data/json";

    const QString filename = QFileDialog::getOpenFileName(this, tr("Open JSON"), path, tr("JSON Files (*.json)"));

    path = QFileInfo(filename).path();

    if(!filename.isEmpty()) {
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

            JsonFile *json = new JsonFile(filename, doc);

            JsonTab *tab = new JsonTab(json, settings);
            open_documents->append(tab);
            in_focus = tab;

            int index = ui->tabs_main->addTab(tab, filename.mid(path.length() + 1));
            ui->tabs_main->setCurrentIndex(index);

            connect(tab->view, &QAbstractItemView::clicked, this, &MainWindow::view_clicked);
        }
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
    if(!in_focus || !text.size() || text[0] != '/') return;

    QByteArray ptr = text.toUtf8();

    if(subtree_models.contains(ptr)) {
        ui->view_object->setModel(subtree_models[ptr]);
        ui->view_object->expand(subtree_models[ptr]->index(0, 0));
    }
    else {
        auto model = subtree(in_focus, ptr, ptr.split('/').last());

        if(model) {
            ui->view_object->setModel(model);
            ui->view_object->expand(model->index(0, 0));
        }
    }
}

QStandardItemModel *MainWindow::subtree(const JsonTab *tab, const QByteArray &ptr, const QByteArray &key)
{
    if(subtree_models.contains(ptr))
        return subtree_models[ptr];

    QStandardItemModel *model = nullptr;

    const Value *obj = Pointer(ptr).Get(*tab->json->doc);

    if(obj && (obj->IsObject() || obj->IsArray())) {
        model = new QStandardItemModel;
        QStandardItem *json = new QStandardItem(key);
        model->appendRow(json);
        in_focus->recurse_json(json, *obj);
    }

    if(model)
        subtree_models[ptr] = model;

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

    auto model = subtree(in_focus, ptr, keys[0]);

    if(!model) {
        keys.pop_front();
        ptr = make_pointer(keys);
        model = subtree(in_focus, ptr, keys[0]);
    }

    if(model) {
        ui->view_object->setModel(model);
        ui->view_object->expand(model->index(0, 0));
    }
}
