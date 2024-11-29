#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QBoxLayout>
#include <QMainWindow>
#include <QToolBar>
#include <QMessageBox>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

    private:
        Ui::mainWindow *ui;

        void initConnections();

    private slots:
        void slotMatrix();
};

#endif
