#include "mainwindow.h"
#include "ui_mainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mainWindow)
{
    this->ui->setupUi(this);
    this->initConnections();
}

MainWindow::~MainWindow()
{
    delete this->ui;
}

void MainWindow::initConnections()
{
    connect(ui->actionmatrix, &QAction::triggered, this, &MainWindow::slotMatrix);
}

void MainWindow::slotMatrix()
{
    QMessageBox::information(this, "YES", "WOW");
}
