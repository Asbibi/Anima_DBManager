#ifndef QLISTWITHSEARCH_H
#define QLISTWITHSEARCH_H

#include "qaugmentedlist.h"

#include <QLineEdit>
#include <QStackedWidget>
#include <QListWidget>

class QListWithSearch : public QWidget
{
    Q_OBJECT

private:
    QLineEdit* mySearchField;
    QStackedWidget* myStackedWidget;
    QAugmentedList* myOriginalList;
    QListWidget* mySearchList;

public:
    explicit QListWithSearch(bool _unique, const QString& _defValue, QWidget *_parent = nullptr);
    void SetDisableRemoveLast(bool _can);
    void ResetValues();
    void AddItemAtEnd_NoEmit(const QString& _label);


signals:
    void SelectionChanged(const int _index);
    void ItemEdited(const int _index, const QString& _value);
    void ItemAdded(const int _index, const QString& _value);
    void ItemDuplicated(const int _index, const int _originalIndex);
    void ItemMoved(const int _indexFrom, const int _indexTo);
    void ItemRemoved(const int _index);

public slots:
    void OnOriginalSelectionChanged(const int _index);
    void OnOriginalItemEdited(const int _index, const QString& _value);
    void OnOriginalItemAdded(const int _index, const QString& _value);
    void OnOriginalItemDuplicated(const int _index, const int _originalIndex);
    void OnOriginalItemMoved(const int _indexFrom, const int _indexTo);
    void OnOriginalItemRemoved(const int _index);

    void OnSearchFieldChanged(const QString& text);
    void OnSearchItemSelected(const QString& text);
};

#endif // QLISTWITHSEARCH_H
