#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTabBar>

#include <QDebug>
#include <QElapsedTimer>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_open_json, &QPushButton::clicked, this, &MainWindow::open_json);
    connect(ui->btn_topbar_open, &QPushButton::clicked, this, &MainWindow::open_json);
    connect(ui->tabs_main->tabBar(), &QTabBar::tabCloseRequested, this, &MainWindow::close_tab);
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

    if((int)documents.size() > index && documents[index]) {
        delete documents[index];
        documents.erase(documents.begin() + index);
    }
}

void MainWindow::open_json()
{
    static QString path = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0);

    const QString filename = QFileDialog::getOpenFileName(this, tr("Open JSON"), path);

    path = QFileInfo(filename).path();

    if(!filename.isEmpty()) {

        QByteArray data = read_file(filename);

        rapidjson::Document *doc = new rapidjson::Document;
        doc->ParseInsitu(data.data());

        if(doc->HasParseError()) {
            delete doc;
            msg_box("Failed to parse JSON from: " + filename, "Invalid JSON");
        }
        else {
            documents.push_back(doc);

            JsonTab *tab = new JsonTab(this, doc);
            int index = ui->tabs_main->addTab(tab, filename.mid(path.length() + 1));
            ui->tabs_main->setCurrentIndex(index);

//            if(ui->tabs_main->widget(0)->objectName() == "tab_open")
//                close_tab(0);
        }
    }
}

void MainWindow::msg_box(const QString msg, const QString title)
{
    QMessageBox msg_box;
    msg_box.setText(msg);
    msg_box.setWindowTitle(title);
    msg_box.setIcon(QMessageBox::Critical);
    msg_box.addButton(QMessageBox::Ok);
    msg_box.exec();
}

QByteArray MainWindow::read_file(const QString filename)
{
    QFile file(filename);

    QByteArray buffer;

    if(file.open(QFile::ReadOnly))
    {
        buffer = file.readAll();
        file.close();
    }

    return buffer;
}

void MainWindow::tweak_ui()
{

}
