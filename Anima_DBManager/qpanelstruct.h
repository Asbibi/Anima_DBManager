#ifndef QPANELSTRUCT_H
#define QPANELSTRUCT_H

#include "qpanelwithcount.h"

#include "qelementhandler.h"
#include "qstructidentity.h"
#include "qtemplatestructure.h"

class StructureDB;

class QPanelStruct : public QPanelWithCount
{
    Q_OBJECT

private:
    QStructIdentity* myStructIdentity;
    QTemplateStructure* myTemplateEditor;
    QElementHandler* myElementHandler;

    StructureDB* GetMyStructureDB();

protected:
    virtual int RefreshItemCount_Internal(const int _tableIndex) override;
    virtual void SetItemCount_Internal(const int _tableIndex, const int _newCount) override;

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
    void OnItemApplied();
    void OnItemRequestUpdate(const int _attrIndexToFocus);

    // Element Handler
    void OnElementSelected(const int _index);
    void OnElementAdded(const int _index);
    void OnElementDuplicated(const int _index, const int _originalIndex);
    void OnElementMoved(const int _indexFrom, const int _indexTo);
    void OnElementRemoved(const int _index);

    // Identity
    void OnNameEdited(const QString& _name);
    void OnAbbrevEdited(QString& _abbrev);
    void OnIconEdited(const IconManager::IconType _iconType);
    void OnColorEdited(const QColor& _color);

};

#endif // QPANELSTRUCT_H
