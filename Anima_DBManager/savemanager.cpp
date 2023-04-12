#include "savemanager.h"

#include "db_manager.h"
#include <QDebug>


SaveManager::SaveManager()
{}
SaveManager::~SaveManager()
{}
SaveManager& SaveManager::GetSaveManager()
{
    static SaveManager singleton = SaveManager();
    return singleton;
}
QString SaveManager::GetSaveFileTempFolder(const QString& _saveFilePath)
{
    return QString(_saveFilePath).replace(_saveFilePath.lastIndexOf('/'), -1, "/temp/");    // can replace -1 by _saveFilePath.length() if nec
}
void SaveManager::SaveFile(const QString& _saveFilePath)
{
    SaveManager::GetSaveManager().SaveFileInternal(_saveFilePath);
}
void SaveManager::OpenFile(const QString& _saveFilePath)
{
    SaveManager::GetSaveManager().OpenFileInternal(_saveFilePath);
}



const QString& SaveManager::GetSaveFileExtension()
{
    static const QString fileExt = "uadb";
    // Stands for Unreal Anima DataBase
    return fileExt;
}
void SaveManager::SaveFileInternal(const QString& _saveFilePath)
{
    // Save String Tables to a single file (values in CSV format)
    // Save Structure templates
    // Save structures as CSV or through the setByTextFormat
    // Zip all files to a single save file

    const QString tempFolderPath = GetSaveFileTempFolder(_saveFilePath);
    const DB_Manager& dbManager = DB_Manager::GetDB_Manager();



    // I. String table save

    const int stringTableCount = dbManager.GetStringTableCount();
    QMap<int, QString> languageCodeMap;
    for (int l = 0; l < SStringHelper::SStringLanguages::Count; l++)
    {
        languageCodeMap.insert(l, SStringHelper::GetLanguageCD((SStringHelper::SStringLanguages)l));
    }
    QString filePath = tempFolderPath + "/ST";    // + ".csv";
    std::ofstream csvFile(filePath.toStdString());
    if (!csvFile)
    {
        qCritical() << "ERROR SAVING DB : temp file " << filePath << " couldn't be created";
        return;
    }

    for (int i = 0; i < stringTableCount; i++)
    {
        const auto* table = dbManager.GetStringTable(i);
        const QString& tableName = table->GetTableName();
        for (int l = 0; l < SStringHelper::SStringLanguages::Count; l++)
        {


            csvFile << "###" << languageCodeMap[l].toStdString() << "---" << tableName.toStdString() << "###\n";
            table->WriteValue_CSV(csvFile, (SStringHelper::SStringLanguages)l);
        }
    }
}

void SaveManager::OpenFileInternal(const QString& _saveFilePath)
{
    // Unzip save file
    // Import String Tables
    // Create the structure Tables from templates
        // For the reference attr, if the referenced table doesn't exist yet, add the attribute in a temp array
    // Fill all the values
        // For reference attr, dont set the value but put it in the myDelayedReferenceValues map
    // Once every structure has been created, set all the reference attributes
}
