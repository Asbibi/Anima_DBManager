#ifndef QPANELSTRING_H
#define QPANELSTRING_H

#include "qpanelbase.h"

class QPanelString : public QPanelBase
{
    Q_OBJECT
public:
    explicit QPanelString(QWidget *parent = nullptr);    
    //virtual void Init() override;

public slots:
    virtual void UpdateItemList() override;

    virtual void OnItemEdited(const int _index, const QString& _value) override;
    virtual void OnItemAdded(const int _index, const QString& _value) override;
    virtual void OnItemDuplicated(const int _index, const int _originalIndex) override;
    virtual void OnItemMoved(const int _indexFrom, const int _indexTo) override;
    virtual void OnItemRemoved(const int _index) override;
};

#endif // QPANELSTRING_H
