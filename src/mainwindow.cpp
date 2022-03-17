#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTabBar>

using namespace rapidjson;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    load_settings();

    connect(ui->btn_open_json, &QPushButton::clicked, this, &MainWindow::open_json);
    connect(ui->btn_topbar_open, &QPushButton::clicked, this, &MainWindow::open_json);
    connect(ui->tabs_main->tabBar(), &QTabBar::tabCloseRequested, this, &MainWindow::close_tab);
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
    // to close a tab you must manually free the tab widget and remove the tab
    QWidget *widget = ui->tabs_main->widget(index);
    ui->tabs_main->removeTab(index);
    delete widget;

    // remove corresponding JsonFile from document vector
    if((int)open_documents.size() > index && open_documents[index]) {
        delete open_documents[index]->doc;
        delete open_documents[index];
        open_documents.erase(open_documents.begin() + index);
    }
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

void MainWindow::open_json()
{
    // set Desktop as default path for file explorer
    static QString path = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0);

    // get filename from file explorer
    const QString filename = QFileDialog::getOpenFileName(this, tr("Open JSON"), path);

    // save path of selected file for reopening file explorer
    path = QFileInfo(filename).path();

    if(!filename.isEmpty()) {
        QByteArray file = read_file(filename);

        if(file.isEmpty())
            display_msg_box("Empty file: " + filename, "Empty File");

        Document *doc = new Document;
        doc->ParseInsitu(file.data());

        // free pointer to document if not valid json
        if(doc && doc->HasParseError()) {
            delete doc;
            display_msg_box("Failed to parse JSON from: " + filename, "Invalid JSON");
        }
        else {
            // store filename and pointer to parsed json in vector
            JsonTab::JsonFile *json = new JsonTab::JsonFile(filename, doc);
            open_documents.emplace_back(json);

            // create json tab widget
            JsonTab *tab = new JsonTab(json, settings);

            // close welcome tab if still open
            QWidget *widget = ui->tabs_main->widget(0);

            if(widget && widget->objectName() == "tab_welcome")
                close_tab(0);

            // add json tab to the main tab widget and switch focus
            QString tab_title = filename.mid(path.length() + 1);
            int index = ui->tabs_main->addTab(tab, tab_title);
            ui->tabs_main->setCurrentIndex(index);
        }
    }
}

void MainWindow::load_settings()
{
    QByteArray json = read_file("../settings/settings.json");

    if(json.isEmpty()) {
        settings = new Settings();
    }
    else {
        Document *doc = new Document;
        doc->ParseInsitu(json.data());

        settings = new Settings(doc);
    }
}

QByteArray MainWindow::read_file(const QString filename)
{
    QFile file(filename);
    QByteArray buffer;

    if(file.open(QFile::ReadOnly)) {
        buffer = file.readAll();
        file.close();
    }

    return buffer;
}
