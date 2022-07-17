#ifndef STRUCTUREDB_H
#define STRUCTUREDB_H

#include "structure.h"
#include "templatestructure.h"
#include <QList>
#include <QObject>

class QTemplateStructure;

class StructureDB : public QObject
{
private:
    friend QTemplateStructure;

    TemplateStructure myTemplate;
    QList<Structure*> myStructures;

    void UpdateTemplate();
    bool CheckIndex(int& index) const;

public:
    StructureDB(const TemplateStructure& _structureTemplate);
    StructureDB(const StructureDB& _another);
    void operator=(const StructureDB& _another);
    ~StructureDB();

    void SetTemplateName(const QString& _name);
    void SetTemplateAttributeName(int _index, const QString& _name);
    void AddStructureAt(int index);
    void RemoveStructureAt(int index);
    void ClearStructures();
    void MoveAttribute(int _indexFrom, int _indexTo);
    void ResetAttributeToDefault(int _attrIndex);
    void ChangeAttributeTemplate(int _attrIndex, AttributeTypeHelper::Type _newType, const AttributeParam& _param);


    const QString& GetTemplateName() const;
    QString GetTemplateColorString() const;
    const TemplateStructure& GetTemplate() const;

    const QList<Structure*>& GetStructures() const { return myStructures; }
    const Structure* GetStructureAt(int index) const;
    int GetStructureCount() const;
    const QString GetStructureRowName(int index) const;
    const QString GetStructureRowName(const Structure* _structure) const;

    void WriteValue_CSV_Table(std::ofstream& file) const;
};

#endif // STRUCTUREDB_H
