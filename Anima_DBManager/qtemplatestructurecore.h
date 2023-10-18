#ifndef QTEMPLATESTRUCTURESIMPLE_H
#define QTEMPLATESTRUCTURESIMPLE_H

#include "qtemplatestructurebase.h"
#include "templatestructure.h"

#include <QList>
#include <QPushButton>
#include <QTabWidget>

class QTemplateStructureCore : public QtemplateStructureBase
{
    Q_OBJECT
private:
    TemplateStructure& myTemplateStruct;
    QList<QTemplateAttributeCore*> myTemplateAttributeCoreList;

    QString ComputeTabNameForAttribute(const AttributeTypeHelper::Type& _type);

protected:
    virtual void AddAttribute(int _position, bool _duplicatePrevious = false) override;

public:
    explicit QTemplateStructureCore(TemplateStructure& _templateStruct, QWidget *parent = nullptr);
    bool HasConfigValid() const;
    void ShowDefaultWidget(bool _show);
    void OnAttributeTypeChanged(const int _index, const AttributeTypeHelper::Type _type);

signals:
    void StructureChanged(bool _criticalChanges = true);

public slots:
    virtual void UpdateContent() override final;

    virtual void OnTabMoved(int _indexFrom, int _indexTo) override;
    virtual void OnAddBefore() override;
    virtual void OnAddAfter() override;
    virtual void OnDuplicate() override;
    virtual void OnRemove() override;

    void OnAttributeEdited(bool _withCriticalChanges);
};

#endif // QTEMPLATESTRUCTURESIMPLE_H
