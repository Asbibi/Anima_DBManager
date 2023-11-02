#include "savemanager.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include "sstringimporter.h"
#include "structureimporthelper.h"


const QByteArray SaveManager::separator = QByteArray::fromStdString("%$%$%$%$%\n");
const QString SaveManager::fileEndString = "1_ST.csv";
const QString SaveManager::fileEndEnum = "2_EN.csv";
const QString SaveManager::fileEndTemplate = "3_TP.json";
const QString SaveManager::fileEndData = "4_DT.json";
const QString SaveManager::fileEndPro = "5_PR.csv";

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
#ifdef REPLACE_TEMP_SAVE_FOLDER
        QDir tempDir(_tempFolderPath);
        tempDir.removeRecursively();
        qWarning() << "Temp Save Folder alreday exists : deleted and recreated";
#else
        QString warningtext = "Needed temporary folder \"" + _tempFolderPath + "\" already exists.\n\nPlease delete it or change your file name before saving again.";
        QMessageBox::information(
            nullptr,
            "Temporary Save Folder already exists",
            warningtext,
            QMessageBox::Ok);
        return false;
#endif
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
        else
        {
            possibleStart = _data.indexOf(separator[0], possibleStart + 1);
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

void SaveManager::New()
{
    DB_Manager::GetDB_Manager().Reset();
    GetSaveManager().myCurrentlyOpenedFile = "";
}
void SaveManager::SaveAuto()
{
    GetSaveManager().SaveAutoInternal();
}
void SaveManager::SaveFile(const QString& _saveFilePath)
{
    SaveManager::GetSaveManager().SaveFileInternal(_saveFilePath);
}
void SaveManager::OpenFile(const QString& _saveFilePath)
{
    SaveManager::GetSaveManager().OpenFileInternal(_saveFilePath);
}
bool SaveManager::IsOpeningFile()
{
    return SaveManager::GetSaveManager().myIsOpening;
}



bool SaveManager::HasCurrentFile()
{
    return GetCurrentSaveFile() != "";
}
const QString& SaveManager::GetCurrentSaveFile()
{
    return GetSaveManager().myCurrentlyOpenedFile;
}
const QString& SaveManager::GetSaveFileExtension()
{
    static const QString fileExt = "uadb";
    // Stands for Unreal Anima DataBase
    return fileExt;
}
void SaveManager::SaveAutoInternal()
{
    if (!HasCurrentFile())
        return;

    qDebug() << "AutoSaving";
    QString autoFileName = myCurrentlyOpenedFile;
    autoFileName.insert(autoFileName.lastIndexOf('.'), "_" + QDateTime::currentDateTime().toString(Qt::ISODate).replace(':', '-'));
    SaveFileInternal(autoFileName, true);
}
void SaveManager::SaveFileInternal(const QString& _saveFilePath, bool _isAutoSave)
{
    // Save String Tables to a single file (values in CSV format)
    // Save enums
    // Save Structure templates
    // Save Structure default attributes
    // Save structures as CSV or through the setByTextFormat
    // Save Project infos
    // Zip all files to a single save file



    // 0. Preparation

    Q_ASSERT(!myIsOpening);
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
            csvStringFile << "###" << languageCodeMap[l].toStdString() << "---" << tableName.toStdString() << "###";
            table->WriteValue_CSV(csvStringFile, (SStringHelper::SStringLanguages)l);
            csvStringFile << '\n';
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



    // III. Save structure templates & Structure defaults

    const int structTableCount = dbManager.GetStructuresCount();
    QJsonArray templateJson = QJsonArray();
    for (int i = 0; i < structTableCount; i++)
    {
        const auto& templateStruct = dbManager.GetStructureTable(i)->GetTemplate();
        templateStruct.SaveTemplate(templateJson);
    }

    QString templateFilePath = tempFolderPath + fileEndTemplate;
    tempFileList << templateFilePath;
    QFile jsonTemplateFile = QFile(templateFilePath);
    if(!jsonTemplateFile.open(QIODevice::ReadWrite))
    {
        qCritical() << "ERROR SAVING DB : default file " << templateFilePath << " couldn't be created";
        return;
    }
    jsonTemplateFile.write(QJsonDocument(templateJson).toJson());
    jsonTemplateFile.close();



    // IV. Save structure datas

    QString structFilePath = tempFolderPath + fileEndData;
    tempFileList << structFilePath;
    QFile jsonStructFile = QFile(structFilePath);
    if(!jsonStructFile.open(QIODevice::ReadWrite))
    {
        qCritical() << "ERROR SAVING DB : temp file " << structFilePath << " couldn't be created";
        return;
    }
    QJsonObject structData = QJsonObject();
    for (int i = 0; i < structTableCount; i++)
    {
        const auto* structTable = dbManager.GetStructureTable(i);
        structData.insert(structTable->GetTemplateName(), structTable->WriteValue_JSON_Table());
        //csvStructFile << "###" << structTable->GetTemplateName().toStdString() << "###\n";
        //structTable->WriteValue_CSV_Table(csvStructFile);
    }
    jsonStructFile.write(QJsonDocument(structData).toJson());
    jsonStructFile.close();



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
    csvProFile << dbManager.GetAttributePrefix().toStdString() << '\n';
    csvProFile << dbManager.GetAttributeSuffix().toStdString() << '\n';
    csvProFile << (dbManager.GetAutoSaveEnabled() ? 1 : 0) << '\n';
    csvProFile << dbManager.GetAutoSaveInterval() << '\n';
    if (dbManager.IsProjectContentFolderPathValid())
    {
        csvProFile << dbManager.GetProjectContentFolderPath().toStdString() << '\n';
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
#ifdef SAVE_WITH_COMPRESSION
    QByteArray compressedData = qCompress(uncompressedData,9);
    saveFile.write(compressedData);
#else
    saveFile.write(uncompressedData);
#endif
    saveFile.close();



    // VII. Clean Up

    QDir tempDir(tempFolderPath);
    tempDir.removeRecursively();

    // VIII. Remember the saved file as the opened one, only if regular save (ie not auto)
    if (!_isAutoSave)
    {
        myCurrentlyOpenedFile = _saveFilePath;
    }
}
void SaveManager::OpenFileInternal(const QString& _saveFilePath)
{
    // Unzip all
    // Use Project infos
    // Import String Tables
    // Import Enums
    // Create the structure Tables from templates
    // Fill all the struct values
    // Set myCurrentlyOpenedFile




    // 0. Preparation

    Q_ASSERT(!myIsOpening);
    myIsOpening = true;
    DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    const QString tempFolderPath = GetSaveFileTempFolder(_saveFilePath);
    if (!TryMakeTempFolder(tempFolderPath))
    {
        return;
    }



    // I. Unzip save file

    QFile saveFile(_saveFilePath);
    saveFile.open(QIODevice::ReadOnly);
#ifdef SAVE_WITH_COMPRESSION
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

    ProcessProjTempFile(tempFolderPath, dbManager);


    // IV. String Table

    ProcessStringTempFile(tempFolderPath);


    // V. Enums

    ProcessEnumTempFile(tempFolderPath, dbManager);


    // VI. Struct Templ

    ProcessTemplTempFile(tempFolderPath, dbManager);


    // VII. Fill Data except refs

    ProcessDataTempFile(tempFolderPath, dbManager);



    // VIII. Clean Up

    QDir tempDir(tempFolderPath);
    tempDir.removeRecursively();
    myIsOpening = false;


    // IX. Remember which file is open
    myCurrentlyOpenedFile = _saveFilePath;
}


void SaveManager::ProcessProjTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager)
{
    QFile projectFile(_tempFolderPath + fileEndPro);
    bool openProCheck = projectFile.open(QIODevice::ReadOnly);
    Q_ASSERT(openProCheck);
    QTextStream proIn(&projectFile);
    QString proFirstLine = proIn.readLine();
    Q_ASSERT(proFirstLine == "###PROJECT_FOLDER###");
    _dbManager.SetAttributePrefix(proIn.readLine());
    _dbManager.SetAttributeSuffix(proIn.readLine());
    const bool autoSaveFile = proIn.readLine() == '1';
    _dbManager.SetAutoSave(autoSaveFile, proIn.readLine().toInt());
    _dbManager.SetProjectContentFolderPath(proIn.readLine());

    Q_ASSERT(proIn.atEnd());
    projectFile.close();
}
void SaveManager::ProcessStringTempFile(const QString& _tempFolderPath)
{
    QMap<QString, SStringImporter> importerMap;

    QFile file(_tempFolderPath + fileEndString);
    bool openCheck = file.open(QIODevice::ReadOnly);
    Q_ASSERT(openCheck);
    QTextStream in(&file);


    QString currentLine;

    QFile* currentFile = nullptr;
    QTextStream* curentFileStream = nullptr;

    while (!in.atEnd())
    {
        currentLine = in.readLine();

        if (currentLine.first(3) == "###" && currentLine.last(3) == "###")
        {
            int separatorStart = currentLine.indexOf("-");
            QString currentLangg = currentLine.mid(3, separatorStart - 3);
            QString currentTable = currentLine.mid(separatorStart + 3, currentLine.length() - 6 - separatorStart);

            QString currentFilePath = _tempFolderPath + currentTable + currentLangg + fileEndString;


            if (currentFile != nullptr)
            {
                currentFile->close();
                delete currentFile;
                delete curentFileStream;
                currentFile = nullptr;
                curentFileStream = nullptr;
            }

            currentFile = new QFile(currentFilePath);
            currentFile->open(QIODevice::WriteOnly);
            curentFileStream = new QTextStream(currentFile);

            if (!importerMap.contains(currentTable))
            {
                importerMap.insert(currentTable, SStringImporter());
            }

            importerMap[currentTable].RegisterLanguageFile(SStringHelper::GetLanguageFromCD(currentLangg), currentFilePath);
        }

        if (curentFileStream != nullptr)
        {
            *curentFileStream << currentLine << '\n';
        }
    }

    if (currentFile != nullptr)
    {
        currentFile->close();
        delete currentFile;
        delete curentFileStream;
        currentFile = nullptr;
        curentFileStream = nullptr;
    }




    auto stringTableNames = importerMap.keys();
    for (const auto& tableName : stringTableNames)
    {
        importerMap[tableName].PerformImport(-1, 0, tableName);
    }

    file.close();
}
void SaveManager::ProcessEnumTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager)
{
    QFile file(_tempFolderPath + fileEndEnum);
    bool openCheck = file.open(QIODevice::ReadOnly);
    Q_ASSERT(openCheck);
    QTextStream in(&file);

    Enumerator currentEnum = Enumerator("");
    QString currentLine;
    bool firstLine = true;
    bool useColor = false;
    while (!in.atEnd())
    {
        currentLine = in.readLine();
        if (currentLine[0] == '#')
        {
            if (firstLine)
            {
                firstLine = false;
            }
            else
            {
                _dbManager.AddEnum(currentEnum);
            }

            currentEnum = Enumerator(currentLine.remove('#'));

            continue;
        }

        int indexOfSeparator = currentLine.indexOf('|');
        if (indexOfSeparator < 0)
        {
            useColor = currentLine.last(4) == "TRUE";
            continue;
        }

        if (useColor)
        {
            QColor color = QColor(currentLine.last(7));
            currentEnum.AddValue(currentLine.first(indexOfSeparator), &color);
        }
        else
        {
            currentEnum.AddValue(currentLine.first(indexOfSeparator));
        }
    }

    if (!firstLine)
    {
        _dbManager.AddEnum(currentEnum);
    }

    file.close();
}
void SaveManager::ProcessTemplTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager)
{
    QFile file(_tempFolderPath + fileEndTemplate);
    bool openCheck = file.open(QIODevice::ReadOnly);
    Q_ASSERT(openCheck);

    const QJsonArray importedJson = QJsonDocument::fromJson(file.readAll()).array();
    file.close();

    // First loop to create the structures and set the "identity" values
    for (const QJsonValue& templAsJson : importedJson)
    {
        _dbManager.AddStructureDB(TemplateStructure::LoadTemplateNoAttribute(templAsJson.toObject()));
    }

    // Second loop to set the attributes (parameters and default values)
    int i = 0;
    for (const QJsonValue& templAsJson : importedJson)
    {
        _dbManager.SetAttributeTemplatesFromJSON(i, templAsJson.toObject().value("Attributes").toArray());
        i++;
    }
}
void SaveManager::ProcessDataTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager)
{
    QFile file(_tempFolderPath + fileEndData);
    bool openCheck = file.open(QIODevice::ReadOnly);
    Q_ASSERT(openCheck);

    const QJsonObject importedJson = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    QStringList structNames = importedJson.keys();
    for (const auto& strctName : structNames)
    {
        StructureDB* currentStructTable = _dbManager.GetStructureTable(strctName);
        Q_ASSERT(currentStructTable != nullptr);
        currentStructTable->ReadValue_JSON_Table(importedJson.value(strctName).toArray(), StructureImportHelper::OverwritePolicy::Overwrite);
    }
}

