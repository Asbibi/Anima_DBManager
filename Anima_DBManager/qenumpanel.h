#ifndef QENUMPANEL_H
#define QENUMPANEL_H

#include <QWidget>

#include "qaugmentedlist.h"
#include <QCheckBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QString>

class QEnumPanel : public QWidget
{
    Q_OBJECT

private:
    QAugmentedList* myItemList;

    QAugmentedList* myEnumValuesList;
    QListWidget* myEnumColorList;
    QCheckBox* myColorCheckbox;

    QPushButton* myApplyBtn;
    QPushButton* myResetBtn;

    void AddColorItem(const QString& _hexColor, int _index = -1);

public:
    explicit QEnumPanel(QWidget *parent = nullptr);

signals:
public slots:
    void UpdateItemList();
    void UpdateEditSection(const int _index);

    void ApplyEdits();
    void RevertEdits();

    void OnItemEdited(const int _index, const QString& _value);
    void OnItemAdded(const int _index, const QString& _value);
    void OnItemMoved(const int _indexFrom, const int _indexTo);
    void OnItemRemoved(const int _index);

    void SetColorFromText(QListWidgetItem* _item);
    void OnAddedEnumValue(int _index);
    void OnMovedEnumValue(int _indexFrom, int _indexTo);
    void OnRemovedEnumValue(int _index);
    void OnToggleUseColor(const int _state);
};

#endif // QENUMPANEL_H
