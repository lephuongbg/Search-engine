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
    void indexStopWordsFile(const string &fileName);
    void addDocuments(vector<string> docNames);
    
private slots:
    void on_actionAddFiles_triggered();

    void on_actionClose_triggered();

    void on_actionAddFolder_triggered();

    void on_actionClear_triggered();

    void on_filterButton_clicked();

    void updateFileView(QStringList &list);

    void updateWordsView();

    void on_byRelevant_clicked();

    void on_byName_clicked();

    void on_actionShowIndexedData_triggered(bool checked);

    void on_wordsView_clicked(const QModelIndex &index);

signals:
    void updatedList(QStringList &list);
    void updatedWordList();

private:
    Ui::MainWindow *ui;
    Indexer * indexer;
    QStringList fileList;
    QStringList resultList;
    QStringListModel displayListModel;
    QStringList wordList;
    QStringListModel wordListModel;

    bool stopWordsAsked;
    void getStopWords();
    void index(QStringList list);
    void indexIndividual(const QString &filename);
    void updateWordList(INode * list);
    void updateStatusBar();
};

#endif // MAINWINDOW_H
