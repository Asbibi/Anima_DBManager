#ifndef QPANELSTRUCT_H
#define QPANELSTRUCT_H

#include "qpanelbase.h"
#include "qtemplatestructure.h"
#include "qelementhandler.h"

class StructureDB;

class QPanelStruct : public QPanelBase
{
    Q_OBJECT
private:
    QTemplateStructure* myTemplateEditor;
    QElementHandler* myElementHandler;

    StructureDB* GetMyStructureDB();

public:
    QPanelStruct(QWidget* parent = nullptr);

public slots:
    virtual void UpdateItemList() override;

    virtual void OnItemSelected(const int _index) override;
    virtual void OnItemEdited(const int _index, const QString& _value) override;
    virtual void OnItemAdded(const int _index, const QString& _value) override;
    virtual void OnItemDuplicated(const int _index, const int _originalIndex) override;
    virtual void OnItemMoved(const int _indexFrom, int _indexTo) override;
    virtual void OnItemRemoved(const int _index) override;

    void OnElementSelected(const int _index);
    void OnElementAdded(const int _index);
    void OnElementDuplicated(const int _index, const int _originalIndex);
    void OnElementMoved(const int _indexFrom, const int _indexTo);
    void OnElementRemoved(const int _index);
};

#endif // QPANELSTRUCT_H
