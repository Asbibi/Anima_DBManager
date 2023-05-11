#include "sstringimporter.h"

#include <QDebug>
#include <QFile>

#include "db_manager.h"


SStringImporter::SStringImporter()
{}

bool SStringImporter::HasNoFileRegistered() const
{
    return myCSVMap.isEmpty();
}

void SStringImporter::RegisterLanguageFile(SStringHelper::SStringLanguages _language, const QString& _filePath)
{
    myCSVMap.insert(_language, _filePath);
}
bool SStringImporter::PerformImport(int _stringTableIndex, int _overrideChoice, const QString& _newTableName)
{
    DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    int stringTableCount = dbManager.GetStringTableCount();
    if (_stringTableIndex == stringTableCount || _stringTableIndex < 0)
    {
        _stringTableIndex = stringTableCount;
        _overrideChoice = 0;

        dbManager.AddStringTable(_newTableName);
        dbManager.GetStringTable(_stringTableIndex)->RemoveStringItem(0);
    }

    return PerformImportOnTable(dbManager.GetStringTable(_stringTableIndex), _overrideChoice);
}
bool SStringImporter::PerformImportOnTable(SStringTable* _stringTable, int _overrideChoice)
{
    Q_ASSERT(_stringTable != nullptr);
    qDebug() << "Import String Table";
    const auto languageKeys = myCSVMap.keys();
    for (const SStringHelper::SStringLanguages& language : languageKeys)
    {
        QFile file(myCSVMap[language]);
        if(!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        QTextStream in(&file);

        int lineNumber = 0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            if (lineNumber == 0)
            {
                lineNumber++;
                continue;
            }
            lineNumber++;

            QStringList fields = line.split("\",\"");
            if (fields.count() != 2)
            {
                qWarning() << "Line " << lineNumber << " skipped because not formatted correctly : " << line;
                continue;
            }

            fields[0].remove(0,1);
            fields[1].remove(fields[1].length()-1,1);
            if (fields[1].isEmpty())
            {
                continue;
            }
            qDebug() << "Importing language: " << SStringHelper::GetLanguageCD(language) << "key: " << fields[0] << " value: " << fields[1];

            _stringTable->ImportString(language, fields[0], fields[1], _overrideChoice);
        }

        file.close();
    }
    return true;
}
