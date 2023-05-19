#ifndef QAUGMENTEDLIST_H
#define QAUGMENTEDLIST_H

#include <QWidget>

#include <QListWidget>
#include <QPushButton>

class QAugmentedList : public QWidget
{
    Q_OBJECT

private:
    QListWidget* myList;
    bool myPropagateChanged = true;
    bool myOnlyUniqueValues;
    bool myDisableRemoveLast = false;
    QString myDefaultValue;
    QPushButton* myRemoveButton;

    void MoveItemAt(int _indexFrom, int _indexTo);
    void RemoveItemAt(int _index);

public:
    explicit QAugmentedList(bool _unique, const QString& _defValue, QWidget *_parent = nullptr);
    void SetDisableRemoveLast(bool _can) { myDisableRemoveLast = _can; }
    void SetItemEditable(bool _editable);
    int Count() const;
    int GetCurrent(QString* _outValue = nullptr) const;
    const QString GetItemValue(const int _index) const;
    void SetItemValue(const int _index, const QString& _value, bool _emitEditedSignal = true);

    void SelectItemAt(const int _index);
    void SelectItem(const QString& _text);

    void Clear();
    void AddItemAt(const QString& _label, int _index = -1, const bool _emit = false, const int _copyFrom = -1);

signals:
    void SelectionChanged(const int _index);
    void ItemEdited(const int _index, const QString& _value);
    void ItemAdded(const int _index, const QString& _value);
    void ItemDuplicated(const int _index, const int _originalIndex);
    void ItemMoved(const int _indexFrom, const int _indexTo);
    void ItemRemoved(const int _index);
    void ItemRemoveCheck(const int _index, bool& _result);

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
