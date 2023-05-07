#include "savemanager.h"

#include "db_manager.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

//#define WITH_COMPRESSION

const QByteArray SaveManager::separator = QByteArray::fromStdString("%$%$%$%$%\n");
const QString SaveManager::fileEndString = "ST.csv";
const QString SaveManager::fileEndEnum = "EN.csv";
const QString SaveManager::fileEndTemplate = "TP.csv";
const QString SaveManager::fileEndData = "DT.csv";
const QString SaveManager::fileEndPro = "PR.csv";

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
bool SaveManager::TryMakeTempFolder(const QString& _tempFolderPath)
{
    if (QFileInfo::exists(_tempFolderPath))
    {
        QString warningtext = "Needed temporary folder \"" + _tempFolderPath + "\" already exists.\n\nPlease delete it or change your file name before saving again.";
        QMessageBox::information(
            nullptr,
            "Temporary Save Folder already exists",
            warningtext,
            QMessageBox::Ok);
        return false;
    }

    QDir().mkdir(_tempFolderPath);
    return true;
}
int SaveManager::FindFileSeparatorStart(const QByteArray& _data, const int _start)
{
    const int separatorlength = separator.length();
    int possibleStart = _data.indexOf(separator[0], _start);
    bool inValidation = true;

    while(inValidation && possibleStart != -1)
    {
        possibleStart = _data.indexOf(separator[0], possibleStart);
        bool ok = true;
        for (int i = 0; i < separatorlength; i++)
        {
            if (separator[i] != _data[i+possibleStart])
            {
                ok = false;
                break;
            }
        }
        if (ok)
        {
            inValidation = false;
        }
    }
    return possibleStart;
}
int SaveManager::WriteTempFileOnOpen(const QByteArray& _data, const QString& _tempFilePath, int _separatorBegin)
{
    const int separatorlength = separator.length();
    int nextSeparator = FindFileSeparatorStart(_data, _separatorBegin + separatorlength);

    QFile tempEnumFile(_tempFilePath);
    tempEnumFile.open(QIODevice::WriteOnly);
    tempEnumFile.write(_data.mid(_separatorBegin + separatorlength, nextSeparator - _separatorBegin - separatorlength));
    tempEnumFile.close();

    return nextSeparator;
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
    if (!TryMakeTempFolder(tempFolderPath))
    {
        return;
    }
    QStringList tempFileList = QStringList();



    // I. String table save

    const int stringTableCount = dbManager.GetStringTableCount();
    QMap<int, QString> languageCodeMap;
    for (int l = 0; l < SStringHelper::SStringLanguages::Count; l++)
    {
        languageCodeMap.insert(l, SStringHelper::GetLanguageCD((SStringHelper::SStringLanguages)l));
    }
    QString stringFilePath = tempFolderPath + fileEndString;
    tempFileList << stringFilePath;
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
    QString enumFilePath = tempFolderPath + fileEndEnum;
    tempFileList << enumFilePath;
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
    QString templFilePath = tempFolderPath + fileEndTemplate;
    tempFileList << templFilePath;
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

    QString structFilePath = tempFolderPath + fileEndData;
    tempFileList << structFilePath;
    std::ofstream csvStructFile(structFilePath.toStdString());
    if (!csvStructFile)
    {
        qCritical() << "ERROR SAVING DB : temp file " << structFilePath << " couldn't be created";
        return;
    }
    for (int i = 0; i < structTableCount; i++)
    {
        const auto* structTable = dbManager.GetStructureTable(i);
        csvStructFile << "###" << structTable->GetTemplateName().toStdString() << "###\n";
        structTable->WriteValue_CSV_Table(csvStructFile);
    }
    csvStructFile.close();



    // V. Save Project Infos

    QString projectFilePath = tempFolderPath + fileEndPro;
    tempFileList << projectFilePath;
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

    QFile saveFile(_saveFilePath);
    saveFile.open(QIODevice::WriteOnly);
    QByteArray uncompressedData;
    for (const auto& file : tempFileList)
    {
        uncompressedData.append(separator);
        QFile infile(file);
        infile.open(QIODevice::ReadOnly);
        uncompressedData.append(infile.readAll());
        infile.close();
    }
#ifdef WITH_COMPRESSION
    QByteArray compressedData = qCompress(uncompressedData,9);
    saveFile.write(compressedData);
#else
    saveFile.write(uncompressedData);
#endif
    saveFile.close();



    // VII. Clean Up

    QDir tempDir(tempFolderPath);
    tempDir.removeRecursively();
}
void SaveManager::OpenFileInternal(const QString& _saveFilePath)
{
    // Unzip all
    // Use Project infos
    // Import String Tables
    // Import Enums
    // Create the structure Tables from templates
        // For the reference attr, if the referenced table doesn't exist yet, add the attribute in a temp array
    // Fill all the values
        // For reference attr, dont set the value but put it in the myDelayedReferenceValues map
    // Once every structure has been created, set all the reference attributes



    // 0. Preparation

    const DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    const int separatorlength = separator.length();
    const QString tempFolderPath = GetSaveFileTempFolder(_saveFilePath);
    if (!TryMakeTempFolder(tempFolderPath))
    {
        return;
    }



    // I. Unzip save file

    QFile saveFile(_saveFilePath);
    saveFile.open(QIODevice::ReadOnly);
#ifdef WITH_COMPRESSION
    QByteArray compressedData = saveFile.readAll();
    QByteArray uncompressedData = qUncompress(compressedData);
#else
    QByteArray uncompressedData = saveFile.readAll();
#endif
    saveFile.close();



    // II. Read Bytes in separate temporary files

    int firstSeparator = FindFileSeparatorStart(uncompressedData, 0);
    int secondSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndString, firstSeparator);
    int thirdSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndEnum, secondSeparator);
    int fourthSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndTemplate, thirdSeparator);
    int fithSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndData, fourthSeparator);
    WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndPro, fithSeparator);



    // III. Project Info
    // IV. String Table
    // V. Enums
    // VI. Struct Templ
    // VII. Fill Data except refs
    // VIII. Fill Refs


    // IX. Clean Up

    QDir tempDir(tempFolderPath);
    tempDir.removeRecursively();
}
