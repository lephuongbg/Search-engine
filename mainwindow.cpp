#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    indexer = new Indexer;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete indexer;
}

void MainWindow::on_actionAddFiles_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec())
         filelist = dialog.selectedFiles();
    qDebug() << filelist;
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}
