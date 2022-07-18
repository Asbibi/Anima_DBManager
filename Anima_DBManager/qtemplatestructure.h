#ifndef QTEMPLATESTRUCTURE_H
#define QTEMPLATESTRUCTURE_H

#include <QWidget>

#include "structuredb.h"
#include "qtemplateattribute.h"

#include <QList>
#include <QPushButton>
#include <QTabWidget>

class QTemplateStructure : public QWidget
{
    Q_OBJECT
private:
    StructureDB* myStructureDB;

    QTabWidget* myTabWidget;
    QList<QString> myAttributeNames;
    QList<QPushButton*> myPushBtns;

    void UpdateAttributeTabText(int index);

public:
    explicit QTemplateStructure(QWidget *parent = nullptr);

    void SetStructureDB(StructureDB* _structureDB);

signals:
public slots:
    void UpdateContent();

    void OnTabMoved(int _indexFrom, int _indexTo);
    void OnNameChanged(const QString& _previousName, QString& _newName);
    void OnApply(const QString& _attrName, AttributeTypeHelper::Type _newType, const AttributeParam& _param, bool _hasCriticalChanges);
    void OnRevert(const QString& _attrName);
    void OnApplyDefaultToAll(const QString& _attrName);

    void OnAddBefore();
    void OnAddAfter();
    void OnDuplicate();
    void OnRemove();
};

#endif // QTEMPLATESTRUCTURE_H
