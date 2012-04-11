#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include "indexer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionAddFiles_triggered();
    void on_actionClose_triggered();
    void updateFileView();

signals:
    void fileListChanged();
private:
    Ui::MainWindow *ui;
    Indexer * indexer;
    QStringList fileList;
    QStringListModel fileListModel;
    bool stopWordsAsked;
    void getStopWords();
    void index(QStringList &list);
};

#endif // MAINWINDOW_H
