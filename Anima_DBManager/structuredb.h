#ifndef STRUCTUREDB_H
#define STRUCTUREDB_H

#include "structure.h"
#include "templatestructure.h"
#include <QList>
#include <QObject>

class StructureDB : public QObject
{
private:
    TemplateStructure myTemplate;
    QList<Structure*> myStructures;

    void UpdateTemplate();
    bool CheckIndex(int& index) const;

public:
    StructureDB(const TemplateStructure& _structureTemplate);
    StructureDB(const StructureDB& _another);
    void operator=(const StructureDB& _another);
    ~StructureDB();

    void AddStructureAt(int index);
    void RemoveStructureAt(int index);
    void ClearStructures();
    const QList<Structure*>& GetStructures() const { return myStructures; }
    // Todo : return count + change vector to QList

    void SetTemplateName(const QString& _name);
    const QString& GetTemplateName() const;
    const TemplateStructure& GetTemplate() const;
    const Structure* GetStructureAt(int index) const;
    int GetStructureCount() const;
    const QString GetStructureRowName(int index) const;
    const QString GetStructureRowName(const Structure* _structure) const;

    void WriteValue_CSV_Table(std::ofstream& file) const;
};

#endif // STRUCTUREDB_H
