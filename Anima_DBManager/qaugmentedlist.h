#ifndef QAUGMENTEDLIST_H
#define QAUGMENTEDLIST_H

#include <QWidget>

#include <QListWidget>

class QAugmentedList : public QWidget
{
    Q_OBJECT

private:
    QListWidget* myList;
    bool myPropagateChanged = true;

    void MoveItemAt(int _indexFrom, int _indexTo);
    void RemoveItemAt(int _index);

public:
    explicit QAugmentedList(QWidget *_parent = nullptr);
    int Count() const;
    int GetCurrent(QString* _outValue = nullptr) const;
    const QString GetItemValue(const int _index) const;

    void Clear();
    void AddItemAt(const QString& _label, int _index = -1, const bool _emit = false);

signals:
    void SelectionChanged(const int _index);
    void ItemEdited(const int _index, const QString& _value);
    void ItemAdded(const int _index, const QString& _value);
    void ItemMoved(const int _indexFrom, const int _indexTo);
    void ItemRemoved(const int _index);

public slots:
    void OnSelectionChanged(const int _index);
    void OnListEdited(QListWidgetItem *item);

    void OnAddBefore();
    void OnAddAfter();
    void OnDuplicate();
    void OnRemove();

    void OnMoveFirst();
    void OnMoveUp();
    void OnMoveDown();
    void OnMoveLast();
};

#endif // QAUGMENTEDLIST_H
