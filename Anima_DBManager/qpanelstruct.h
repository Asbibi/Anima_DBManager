#ifndef QPANELSTRUCT_H
#define QPANELSTRUCT_H

#include "qpanelbase.h"
#include "qtemplatestructure.h"

class QPanelStruct : public QPanelBase
{
    Q_OBJECT
private:
    QTemplateStructure* myTemplateEditor;

public:
    QPanelStruct(QWidget* parent = nullptr);

public slots:
    virtual void UpdateItemList() override;

    virtual void OnItemSelected(const int _index) override;
    virtual void OnItemEdited(const int _index, const QString& _value) override;
    virtual void OnItemAdded(const int _index, const QString& _value) override;
    virtual void OnItemDuplicated(const int _index, const int _originalIndex) override;
    virtual void OnItemMoved(const int _indexFrom, const int _indexTo) override;
    virtual void OnItemRemoved(const int _index) override;
};

#endif // QPANELSTRUCT_H
