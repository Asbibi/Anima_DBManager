#ifndef QTEMPLATESTRUCTURE_H
#define QTEMPLATESTRUCTURE_H

#include "structuredb.h"
#include "qtemplateattribute.h"
#include "qtemplatestructurebase.h"

#include <QList>
#include <QPushButton>
#include <QTabWidget>

class QTemplateStructure : public QtemplateStructureBase
{
    Q_OBJECT
private:
    TemplateStructure* myTemplateStructureCopy;
    QList<QString> myAttributeNames;


protected:
    virtual void AddAttribute(int _position, bool _duplicatePrevious = false) override;

public:
    explicit QTemplateStructure(QWidget *parent = nullptr);

    void SetStructureDB(const StructureDB* _structureDB);
    void FocusAttribute(const int _index);

signals:
    void AttributeChangeApplied();
    void RequestUpdateTemplateCopy(const int _attrIndexToFocus);

public slots:
    virtual void UpdateContent() override final;

    virtual void OnTabMoved(int _indexFrom, int _indexTo) override;
    virtual void OnAddBefore() override;
    virtual void OnAddAfter() override;
    virtual void OnDuplicate() override;
    virtual void OnRemove() override;

    void OnNameChanged(const QString& _previousName, QString& _newName);
    void OnApply(const QString& _attrName,  const TemplateAttribute& _editedTemplateCopy, bool _hasCriticalChanges);
    void OnRevert(const QString& _attrName);
    void OnApplyDefaultToAll(const QString& _attrName);
};

#endif // QTEMPLATESTRUCTURE_H
