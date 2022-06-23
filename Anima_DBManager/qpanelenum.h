#ifndef QPANELENUM_H
#define QPANELENUM_H

#include "qpanelbase.h"

#include "qaugmentedlist.h"
#include <QCheckBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QString>

class QPanelEnum : public QPanelBase
{
    Q_OBJECT

private:
    QAugmentedList* myEnumValuesList;
    QListWidget* myEnumColorList;
    QCheckBox* myColorCheckbox;

    QPushButton* myApplyBtn;
    QPushButton* myResetBtn;

    void AddColorItem(const QString& _hexColor, int _index = -1);

public:
    explicit QPanelEnum(QWidget *parent = nullptr);
    virtual void Init() override;

public slots:
    virtual void UpdateItemList() override;

    void ApplyEdits();
    void RevertEdits();

    virtual void OnItemSelected(const int _index) override;
    virtual void OnItemEdited(const int _index, const QString& _value) override;
    virtual void OnItemAdded(const int _index, const QString& _value) override;
    virtual void OnItemDuplicated(const int _index, const int _originalIndex) override;
    virtual void OnItemMoved(const int _indexFrom, const int _indexTo) override;
    virtual void OnItemRemoved(const int _index) override;

    void SetColorFromText(QListWidgetItem* _item);
    void OnAddedEnumValue(int _index);
    void OnDuplicatedEnumValue(int _index, int _originalIndex);
    void OnMovedEnumValue(int _indexFrom, int _indexTo);
    void OnRemovedEnumValue(int _index);
    void OnToggleUseColor(const int _state);
    void OnSelectEnumValue(const int _index);
};

#endif // QPANELENUM_H
