#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    indexer = new Indexer;
    stopWordsAsked = false;
    ui->fileView->setModel(&this->fileListModel);
    connect(this, SIGNAL(fileListChanged()), this, SLOT(updateFileView()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete indexer;
}

void MainWindow::on_actionAddFiles_triggered()
{
    if (!stopWordsAsked)
    {
        getStopWords();
        stopWordsAsked = true;
    }

    QFileDialog dialog(this);
    // Select files for indexing
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Text files (*)"));
    if (dialog.exec())
    {
        QStringList newlist = dialog.selectedFiles();
        this->index(newlist);
    }
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::updateFileView()
{
    this->fileListModel.setStringList(fileList);
}

void MainWindow::index(QStringList &list)
{
    // Index selected files
    for (QStringList::Iterator it = list.begin(); it != list.end(); it++)
    {
        // Do not reindex indexed file
        if (!fileList.contains(*it))
        {
            indexer->addDocument(it->toStdString());
            fileList.append(*it);
            qDebug() << *it;
            emit this->fileListChanged();
        }
    }
}

void MainWindow::getStopWords()
{
    QFileDialog dialog(this);

    // Load stop words list
    QMessageBox msbox(this);
    msbox.setText("Do you want to load stop words list first?");
    msbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msbox.setDefaultButton(QMessageBox::Ok);

    if (msbox.exec() == QMessageBox::Ok)
    {
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter(tr("Stop words list (stopwords)"));
        if (dialog.exec())
            indexer->indexStopWords(dialog.selectedFiles().at(0).toStdString());
    }
}
