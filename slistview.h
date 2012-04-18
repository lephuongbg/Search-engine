#ifndef SLISTVIEW_H
#define SLISTVIEW_H

#include <QListView>

class SListView : public QListView
{
    Q_OBJECT
public:
    explicit SListView(QWidget *parent = 0);
    
signals:
    
public slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // SLISTVIEW_H
