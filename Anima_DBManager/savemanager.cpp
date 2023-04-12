#include "savemanager.h"

#include "db_manager.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>


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
    if (!_saveFilePath.endsWith(GetSaveFileExtension()))
        return "";
    return QString(_saveFilePath).replace(_saveFilePath.lastIndexOf('.'), GetSaveFileExtension().length() + 1, "__TEMP__/");    // can replace -1 by _saveFilePath.length() if nec
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
    // Save enums
    // Save Structure templates
    // Save structures as CSV or through the setByTextFormat
    // Save Project infos
    // Zip all files to a single save file


    // 0. Preparation

    const DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    const QString tempFolderPath = GetSaveFileTempFolder(_saveFilePath);
    if (QFileInfo::exists(tempFolderPath))
    {
        QString warningtext = "Needed temporary folder \"" + tempFolderPath + "\" already exists.\n\nPlease delete it or change your file name before saving again.";
        QMessageBox::information(
            nullptr,
            "Temporary Save Folder already exists",
            warningtext,
            QMessageBox::Ok);
        return;
    }
    QDir().mkdir(tempFolderPath);



    // I. String table save

    const int stringTableCount = dbManager.GetStringTableCount();
    QMap<int, QString> languageCodeMap;
    for (int l = 0; l < SStringHelper::SStringLanguages::Count; l++)
    {
        languageCodeMap.insert(l, SStringHelper::GetLanguageCD((SStringHelper::SStringLanguages)l));
    }
    QString stringFilePath = tempFolderPath + "ST.csv";
    std::ofstream csvStringFile(stringFilePath.toStdString());
    if (!csvStringFile)
    {
        qCritical() << "ERROR SAVING DB : temp file " << stringFilePath << " couldn't be created";
        return;
    }

    for (int i = 0; i < stringTableCount; i++)
    {
        const auto* table = dbManager.GetStringTable(i);
        const QString& tableName = table->GetTableName();
        for (int l = 0; l < SStringHelper::SStringLanguages::Count; l++)
        {
            csvStringFile << "###" << languageCodeMap[l].toStdString() << "---" << tableName.toStdString() << "###\n";
            table->WriteValue_CSV(csvStringFile, (SStringHelper::SStringLanguages)l);
        }
    }
    csvStringFile.close();


    // II. Save enums

    const int enumCount = dbManager.GetEnumCount();
    QString enumFilePath = tempFolderPath + "EN.csv";
    std::ofstream csvEnumFile(enumFilePath.toStdString());
    if (!csvEnumFile)
    {
        qCritical() << "ERROR SAVING DB : temp file " << enumFilePath << " couldn't be created";
        return;
    }
    for (int i = 0; i < enumCount; i++)
    {
        const auto* enumerator = dbManager.GetEnum(i) ;
        enumerator->SaveEnum_CSV(csvEnumFile);
    }
    csvEnumFile.close();


    // III. Save structure templates

    const int structTableCount = dbManager.GetStructuresCount();
    QString templFilePath = tempFolderPath + "TP.csv";
    std::ofstream csvTemplFile(templFilePath.toStdString());
    if (!csvTemplFile)
    {
        qCritical() << "ERROR SAVING DB : temp file " << templFilePath << " couldn't be created";
        return;
    }
    for (int i = 0; i < structTableCount; i++)
    {
        const auto& templateStruct = dbManager.GetStructureTable(i)->GetTemplate();
        templateStruct.SaveTemplate_CSV(csvTemplFile);
    }
    csvTemplFile.close();


    // IV. Save structure datas


    // V. Save Project Infos

    QString projectFilePath = tempFolderPath + "PRO.csv";
    std::ofstream csvProFile(projectFilePath.toStdString());
    if (!csvProFile)
    {
        qCritical() << "ERROR SAVING DB : temp file " << projectFilePath << " couldn't be created";
        return;
    }
    csvProFile << "###PROJECT_FOLDER###\n";
    if (dbManager.IsProjectContentFolderPathValid())
    {
        csvProFile << dbManager.GetProjectContentFolderPath().toStdString() << "\n";
    }
    csvProFile.close();


    // VI. Compress all temp files in the final save file


    // VII. Clean Up

    QDir tempDir(tempFolderPath);
    tempDir.removeRecursively();
}

void SaveManager::OpenFileInternal(const QString& _saveFilePath)
{
    // Unzip save file
    // Use Project infos
    // Import String Tables
    // Import Enums
    // Create the structure Tables from templates
        // For the reference attr, if the referenced table doesn't exist yet, add the attribute in a temp array
    // Fill all the values
        // For reference attr, dont set the value but put it in the myDelayedReferenceValues map
    // Once every structure has been created, set all the reference attributes
}
