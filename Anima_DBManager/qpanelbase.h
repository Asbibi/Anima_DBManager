#ifndef QPANELBASE_H
#define QPANELBASE_H

#include <QWidget>

#include "qaugmentedlist.h"
#include <QGroupBox>

class QPanelBase : public QWidget
{
    Q_OBJECT

protected:
    QAugmentedList* myItemList;
    QGroupBox* mySubGroupBox;

public:
    explicit QPanelBase(const QString& itemBoxName, bool _uniqueNames, QWidget *parent = nullptr);
    virtual void Init();

    int GetSelectedItem() const;
    void Reset();
    void UnselectItem();

signals:
public slots:
    virtual void UpdateItemList() = 0;

    virtual void OnItemSelected(const int _index) = 0;
    virtual void OnItemEdited(const int _index, const QString& _value) = 0;
    virtual void OnItemAdded(const int _index, const QString& _value) = 0;
    virtual void OnItemDuplicated(const int _index, const int _originalIndex) = 0;
    virtual void OnItemMoved(const int _indexFrom, const int _indexTo) = 0;
    virtual void OnItemRemoved(const int _index) = 0;
};

#endif // QPANELBASE_H
