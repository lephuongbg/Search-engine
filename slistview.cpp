#include "slistview.h"
#include <QDebug>

SListView::SListView(QWidget *parent) :
    QListView(parent)
{
}

void SListView::selectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    // Emulate click when change selection in the list view
    emit this->clicked(selected.indexes().at(0));
}
