#ifndef QPANELSTRUCT_H
#define QPANELSTRUCT_H

#include "qpanelbase.h"

#include "qelementhandler.h"
#include "qstructidentity.h"
#include "qtemplatestructure.h"

#include <QSpinBox>

class StructureDB;

class QPanelStruct : public QPanelBase
{
    Q_OBJECT

private:
    QStructIdentity* myStructIdentity;
    QTemplateStructure* myTemplateEditor;
    QElementHandler* myElementHandler;
    QSpinBox* myRowCountSpinner;

    StructureDB* GetMyStructureDB();

public:
    QPanelStruct(QWidget* parent = nullptr);
    void RefreshItemCount(const int _count);

public slots:
    virtual void UpdateItemList() override;

    virtual void OnItemSelected(const int _index) override;
    virtual void OnItemEdited(const int _index, const QString& _value) override;
    virtual void OnItemAdded(const int _index, const QString& _value) override;
    virtual void OnItemDuplicated(const int _index, const int _originalIndex) override;
    virtual void OnItemMoved(const int _indexFrom, int _indexTo) override;
    virtual void OnItemRemoved(const int _index) override;
    void OnItemApplied();

    // Element Handler
    void OnElementSelected(const int _index);
    void OnElementAdded(const int _index);
    void OnElementDuplicated(const int _index, const int _originalIndex);
    void OnElementMoved(const int _indexFrom, const int _indexTo);
    void OnElementRemoved(const int _index);

    // Identity
    void OnNameEdited(const QString& _name);
    void OnAbbrevEdited(const QString& _abbrev);
    void OnIconEdited(const IconManager::IconType _iconType);
    void OnColorEdited(const QColor& _color);

    void SetItemCount();
};

#endif // QPANELSTRUCT_H
