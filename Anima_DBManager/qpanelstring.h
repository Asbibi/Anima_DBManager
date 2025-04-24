#ifndef QPANELSTRING_H
#define QPANELSTRING_H

#include "qpanelwithcount.h"
#include "qlistwithsearch.h"

class SStringTable;


class QPanelString : public QPanelWithCount
{
    Q_OBJECT
private:
    QListWithSearch* mySearchList;
    bool myFocusOndictionary = false;

    void LoadStringTable(const SStringTable* _table);

protected:
    virtual int RefreshItemCount_Internal(const int _tableIndex) override;
    virtual void SetItemCount_Internal(const int _tableIndex, const int _newCount) override;
    void SortCurrentTable(bool _ascending);

public:
    explicit QPanelString(QWidget *parent = nullptr);

public slots:
    virtual void UpdateItemList() override;

    virtual void OnItemSelected(const int _index) override;
    virtual void OnItemEdited(const int _index, const QString& _value) override;
    virtual void OnItemAdded(const int _index, const QString& _value) override;
    virtual void OnItemDuplicated(const int _index, const int _originalIndex) override;
    virtual void OnItemMoved(const int _indexFrom, const int _indexTo) override;
    virtual void OnItemRemoved(const int _index) override;

    void OnSubItemSelected(const int _index);
    void OnSubItemEdited(const int _index, const QString& _value);
    void OnSubItemAdded(const int _index, const QString& _value);
    void OnSubItemDuplicated(const int _index, const int _originalIndex);
    void OnSubItemMoved(const int _indexFrom, const int _indexTo);
    void OnSubItemRemoved(const int _index);

    void OnSortAscClicked();
    void OnSortDescClicked();

    void OnSelectDictionary();
};

#endif // QPANELSTRING_H
