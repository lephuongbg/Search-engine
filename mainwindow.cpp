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

    // Connect each list view to its corresponding model
    ui->fileView->setModel(&this->displayListModel);
    ui->wordsView->setModel(&this->wordListModel);

    // Set the labels for the containers view
    QStringList labels;
    labels << tr("File name") << tr("Rank");
    ui->containersView->setHorizontalHeaderLabels(labels);

    connect(this, SIGNAL(updatedList(QStringList&)), this, SLOT(updateFileView(QStringList&)));
    connect(this, SIGNAL(updatedWordList()), this, SLOT(updateWordsView()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete indexer;
}

// ALLOW GET STOP WORDS FILE NAME FROM OUTSIDE THE CLASS
void MainWindow::indexStopWordsFile(const string &fileName)
{
    indexer->indexStopWords(fileName);
    stopWordsAsked = true;
}

// ALLOW GET DOCUMENTS LIST FROM OUTSIDE THE CLASS
void MainWindow::addDocuments(vector<string> docNames)
{
    QStringList newDocNames;
    for (vector<string>::iterator it = docNames.begin(); it != docNames.end(); it++)
        newDocNames.append(QString::fromStdString(*it));
    index(newDocNames);
}

// SELECT FILES FROM THE FILE SYSTEM
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

// CLOSE THE PROGRAM
void MainWindow::on_actionClose_triggered()
{
    this->close();
}

// INDEX ALL SELECTED FILES
void MainWindow::index(QStringList list)
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
            emit updatedList(fileList);
        }
    }
    updateWordList(this->indexer->indexer());
    emit updatedWordList();
}

// UPDATE THE LIST OF ALL INDEXED WORDS
void MainWindow::updateWordList(INode *list)
{
    if (list == indexer->indexer())
        wordList.clear();
    if (list != NULL)
    {
        updateWordList(list->left());
        wordList.append(QString::fromStdString(list->data()->word()));
        updateWordList(list->right());
    }
}

// ADD STOP WORDS LIST TO THE INDEXER
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

// CHOOSE FOLDER TO SELECT ALL FILES IN IT
void MainWindow::on_actionAddFolder_triggered()
{
    if (!stopWordsAsked)
    {
        getStopWords();
        stopWordsAsked = true;
    }

    QFileDialog dialog(this);
    // Select files for indexing
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    if (dialog.exec())
    {
        QStringList newlist = dialog.selectedFiles();
        QDir folder(newlist.at(0));
        newlist = folder.entryList(QDir::Files | QDir::NoDotAndDotDot);
        // Stop if the folder is empty
        if (newlist.size() == 0)
            return;
        for (QStringList::Iterator it = newlist.begin(); it != newlist.end(); it++)
            it->prepend(folder.absolutePath().append("/"));
        this->index(newlist);
    }
}

// RESET THE PROGRAM TO INITIAL STATE
void MainWindow::on_actionClear_triggered()
{
    // Clear files view
    fileList.clear();
    emit updatedList(fileList);

    // Clear words view
    wordList.clear();
    emit updatedWordList();

    // Clear containers view
    int i = ui->containersView->rowCount();
    while (i >= 0)
    {
        ui->containersView->removeRow(i);
        i--;
    }

    // Clear query
    ui->queryEdit->clear();

    // Disable sort options
    ui->byName->setEnabled(false);
    ui->byRelevant->setEnabled(false);

    // Clear indexer
    delete indexer;
    indexer = new Indexer;

    // Reset stopWordsAsked bool value
    stopWordsAsked = false;
}

// EXECUTE THE QUERY FROM QUERY BOX
void MainWindow::on_filterButton_clicked()
{
    QString query(ui->queryEdit->text());

    // If the query is empty, redisplay all indexed files
    if (query.size() == 0)
    {
        emit updatedList(fileList);
        // Disable sort options
        ui->byName->setEnabled(false);
        ui->byRelevant->setEnabled(false);
        return;
    }

    // Add query to the indexer and excute it
    indexer->setQuery(query.toStdString());
    indexer->execute();

    // Display result
    vector<Document> rankSortedResult = indexer->result();
    resultList.clear();
    for (vector<Document>::iterator it = rankSortedResult.begin(); it != rankSortedResult.end(); it++)
        resultList.append(QString::fromStdString(it->name()));
    emit updatedList(resultList);

    // Enable sort options
    ui->byName->setEnabled(true);
    ui->byRelevant->setEnabled(true);
}

// UPDATE THE FILE LIST VIEW
void MainWindow::updateFileView(QStringList &list)
{
    // Only keep file names from the path list
    for (QStringList::Iterator it = list.begin(); it != list.end(); it++)
    {
        int lastSlashPosition = it->lastIndexOf('/');
        it->remove(0, lastSlashPosition+1);
    }
    displayListModel.setStringList(list);
}

// UPDATE THE INDEXED WORDS VIEW
void MainWindow::updateWordsView()
{
    wordListModel.setStringList(wordList);
}

// SORT BY RELEVANT
void MainWindow::on_byRelevant_clicked()
{
    // Redisplay the old result because it's already sorted by relevant
    updateFileView(resultList);
}

// SORT BY NAME
void MainWindow::on_byName_clicked()
{
    displayListModel.sort(0);
}

// SHOW/HIDE INDEXED DATA STACK WIDGET
void MainWindow::on_actionShowIndexedData_triggered(bool checked)
{
    if (checked)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}

// SHOW RELATING INFORMATION WHEN EACH WORD IS SELECTED
void MainWindow::on_wordsView_clicked(const QModelIndex &index)
{
    // Get the containers information for the word
    vector<Document> containerResult;
    containerResult = indexer->at(wordListModel.stringList().at(index.row()).toStdString())->data()->docs();

    // Clear the containers view
    int i = ui->containersView->rowCount();
    while (i >= 0)
    {
        ui->containersView->removeRow(i);
        i--;
    }

    // Iterate through all container of the word
    i = 0;
    vector<Document>::iterator it = containerResult.begin();
    while (it != containerResult.end())
    {
        QTableWidgetItem * filename = new QTableWidgetItem;
        QTableWidgetItem * filerank = new QTableWidgetItem;
        // Only show the name of the file
        QString name(QString::fromStdString(it->name()));
        name.remove(0, name.lastIndexOf('/') + 1);

        // Add row to the view
        ui->containersView->insertRow(i);

        // Display container name
        filename->setText(name);
        ui->containersView->setItem(i, 0, filename);

        // Display container rank
        filerank->setText(QString::number(it->occurrence()));
        ui->containersView->setItem(i, 1, filerank);

        it++;
        i++;
    }
}
