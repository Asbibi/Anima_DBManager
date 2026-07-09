#ifndef SSTRINGIMPORTER_H
#define SSTRINGIMPORTER_H

#include <QMap>
#include "sstringhelper.h"
#include "sstringtable.h"

class SStringImporter
{

private:
    QMap<int, QString> myCSVMap;
    bool PerformImportOnTable(SStringTable* _stringTable, int _overrideChoice);

public:
    SStringImporter();

    bool HasNoFileRegistered() const;

    void RegisterLanguageFile(int _languageIndex, const QString& _filePath);
    bool PerformImport(int _stringTableIndex, int _overrideChoice, const QString& _newTableName);
};

#endif // SSTRINGIMPORTER_H
