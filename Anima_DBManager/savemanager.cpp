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
const QString SaveManager::fileEndLang = "1_LG.csv";
const QString SaveManager::fileEndString = "2_ST.csv";
const QString SaveManager::fileEndEnum = "3_EN.csv";
const QString SaveManager::fileEndTemplate = "4_TP.json";
const QString SaveManager::fileEndData = "5_DT.json";
const QString SaveManager::fileEndPro = "6_PR.csv";

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
    SaveManager::GetSaveManager().SetCurrentlyOpenedFile("");
    SaveManager::GetSaveManager().myHasUnsavedChanges = false;
}
void SaveManager::SaveAuto()
{
    GetSaveManager().SaveAutoInternal();
}
void SaveManager::SaveFile(const QString& _saveFilePath)
{
    SaveManager::GetSaveManager().mySaveFeedbackComponent.StartSaveOpenFeedback();
    SaveManager::GetSaveManager().SaveFileInternal(_saveFilePath);
    SaveManager::GetSaveManager().mySaveFeedbackComponent.EndSaveOpenFeedback();
    SaveManager::GetSaveManager().myHasUnsavedChanges = false;
    DB_Manager::GetDB_Manager().NotifySavePerformed();
}
void SaveManager::OpenFile(const QString& _saveFilePath)
{
    SaveManager::GetSaveManager().mySaveFeedbackComponent.StartSaveOpenFeedback();
    SaveManager::GetSaveManager().OpenFileInternal(_saveFilePath);
    SaveManager::GetSaveManager().mySaveFeedbackComponent.EndSaveOpenFeedback();
    SaveManager::GetSaveManager().myHasUnsavedChanges = false;
    DB_Manager::GetDB_Manager().NotifySavePerformed();
}
bool SaveManager::IsOpeningFile()
{
    return SaveManager::GetSaveManager().myIsOpening;
}
bool SaveManager::IsSavingFile()
{
    return SaveManager::GetSaveManager().myIsSaving;
}



bool SaveManager::HasCurrentFile()
{
    return GetCurrentSaveFile() != "";
}
const QString& SaveManager::GetCurrentSaveFile()
{
    return GetSaveManager().myCurrentlyOpenedFile;
}
void SaveManager::SetCurrentlyOpenedFile(const QString& _file)
{
    myCurrentlyOpenedFile = QDir::fromNativeSeparators(_file);
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

    Q_ASSERT(!myIsOpening && !myIsSaving);
    myIsSaving = true;
    const DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    const QString tempFolderPath = GetSaveFileTempFolder(_saveFilePath);
    if (!TryMakeTempFolder(tempFolderPath))
    {
        return;
    }
    QStringList tempFileList = QStringList();



    // I. Languages save

    const auto& languages = dbManager.GetLanguages();
    const int languagesCount = languages.GetLanguageCount();
    QString languageFilePath = tempFolderPath + fileEndLang;
    tempFileList << languageFilePath;
    std::ofstream csvLanguageFile(languageFilePath.toStdString());
    if (!csvLanguageFile)
    {
        qCritical() << "ERROR SAVING DB : temp file " << languageFilePath << " couldn't be created";
        return;
    }

    mySaveFeedbackComponent.SetSaveLanguageProgress(languagesCount);
    for (int i = 0; i < languagesCount; i++)
    {
        const auto& l = languages.GetLanguage(i);
        csvLanguageFile << l.GetAbbrev().toStdString() << " - " << l.GetName().toStdString()  << '\n';
    }
    csvLanguageFile.close();


    // II. String table save

    const int stringTableCount = dbManager.GetStringTableCount() + 1; // +1 to include the DICTIONARY
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
        mySaveFeedbackComponent.SetSaveStringTableProgress(i, stringTableCount);
        const auto* table = dbManager.GetStringTable(i);
        const QString& tableName = table->GetTableName();
        for (int l = 0; l < languagesCount; l++)
        {
            csvStringFile << "###" << languages.GetLanguage(l).GetAbbrev().toStdString() << "---" << tableName.toStdString() << "###";
            table->WriteValue_CSV(csvStringFile, l, false);
            csvStringFile << '\n';
        }
    }
    csvStringFile.close();



    // III. Save enums

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
        mySaveFeedbackComponent.SetSaveEnumProgress(i, enumCount);
        const auto* enumerator = dbManager.GetEnum(i) ;
        enumerator->SaveEnum_CSV(csvEnumFile);
    }
    csvEnumFile.close();



    // IV. Save structure templates & Structure defaults

    QString templateFilePath = tempFolderPath + fileEndTemplate;
    tempFileList << templateFilePath;
    QFile jsonTemplateFile = QFile(templateFilePath);
    if(!jsonTemplateFile.open(QIODevice::ReadWrite))
    {
        qCritical() << "ERROR SAVING DB : default file " << templateFilePath << " couldn't be created";
        return;
    }    
    const int structTableCount = dbManager.GetStructuresCount();
    QJsonArray templateJson = QJsonArray();
    for (int i = 0; i < structTableCount; i++)
    {
        mySaveFeedbackComponent.SetSaveStructTemplateProgress(i, structTableCount);
        const auto& templateStruct = dbManager.GetStructureTable(i)->GetTemplate();
        templateStruct.SaveTemplate(templateJson);
    }
    jsonTemplateFile.write(QJsonDocument(templateJson).toJson());
    jsonTemplateFile.close();



    // V. Save structure datas

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
        mySaveFeedbackComponent.SetSaveStructDataProgress(i, structTableCount);
        const auto* structTable = dbManager.GetStructureTable(i);
        structData.insert(structTable->GetTemplateName(), structTable->WriteValue_JSON_Table());
        //csvStructFile << "###" << structTable->GetTemplateName().toStdString() << "###\n";
        //structTable->WriteValue_CSV_Table(csvStructFile);
    }
    jsonStructFile.write(QJsonDocument(structData).toJson());
    jsonStructFile.close();



    // VI. Save Project Infos

    QString projectFilePath = tempFolderPath + fileEndPro;
    tempFileList << projectFilePath;
    std::ofstream csvProFile(projectFilePath.toStdString());
    if (!csvProFile)
    {
        qCritical() << "ERROR SAVING DB : temp file " << projectFilePath << " couldn't be created";
        return;
    }
    mySaveFeedbackComponent.SetSaveProjectProgress();
    csvProFile << "###PROJECT_FOLDER###\n";
    csvProFile << dbManager.GetAttributePrefix().toStdString() << '\n';
    csvProFile << dbManager.GetAttributeSuffix().toStdString() << '\n';
    csvProFile << (dbManager.GetAutoSaveEnabled() ? 1 : 0) << '\n';
    csvProFile << dbManager.GetAutoSaveInterval() << '\n';
    csvProFile << (dbManager.IsProjectContentFolderPathValid() ? dbManager.GetRawProjectContentFolderPath() : "").toStdString() << '\n';
    for (const auto& assetType : AttributeTypeHelper::assetTypes)
    {
        csvProFile << dbManager.GetAAssetRegex(assetType).toStdString() << '\n';
    }
    csvProFile.close();



    // VII. Compress all temp files in the final save file

    QFile saveFile(_saveFilePath);
    saveFile.open(QIODevice::WriteOnly);
    QByteArray uncompressedData;
    int fileIndex = 0;
    for (const auto& file : tempFileList)
    {
        mySaveFeedbackComponent.SetSaveCompilationProgress(fileIndex, tempFileList.length());

        uncompressedData.append(separator);
        QFile infile(file);
        infile.open(QIODevice::ReadOnly);
        uncompressedData.append(infile.readAll());
        infile.close();

        fileIndex++;
    }
#ifdef SAVE_WITH_COMPRESSION
    QByteArray compressedData = qCompress(uncompressedData,9);
    saveFile.write(compressedData);
#else
    saveFile.write(uncompressedData);
#endif
    saveFile.close();



    // VIII. Clean Up

    QDir tempDir(tempFolderPath);
    tempDir.removeRecursively();
    myIsSaving = false;


    // IX. Remember the saved file as the opened one, only if regular save (ie not auto)

    if (!_isAutoSave)
    {
        myCurrentlyOpenedFile = _saveFilePath;
    }
}
void SaveManager::OpenFileInternal(const QString& _saveFilePath)
{
    // Set myCurrentlyOpenedFile
    // Unzip all
    // Use Project infos
    // Import String Tables
    // Import Enums
    // Create the structure Tables from templates
    // Fill all the struct values




    // 0. Preparation

    Q_ASSERT(!myIsOpening && !myIsSaving);
    myIsOpening = true;
    DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    const QString tempFolderPath = GetSaveFileTempFolder(_saveFilePath);
    if (!TryMakeTempFolder(tempFolderPath))
    {
        return;
    }

    // X. Remember which file is open
    SetCurrentlyOpenedFile(_saveFilePath);


    // I. Unzip save file

    QFile saveFile(_saveFilePath);
    saveFile.open(QIODevice::ReadOnly);
    QByteArray compressedData = saveFile.readAll();
    QByteArray uncompressedData;
    if (compressedData.startsWith(separator))
    {
        uncompressedData = compressedData;
    }
    else
    {
        uncompressedData = qUncompress(compressedData);
    }
    saveFile.close();



    // II. Read Bytes in separate temporary files

    mySaveFeedbackComponent.SetOpenDecompilationProgress(0, 7);
    int firstSeparator = FindFileSeparatorStart(uncompressedData, 0);

    mySaveFeedbackComponent.SetOpenDecompilationProgress(1, 7);
    int secondSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndLang, firstSeparator);
    mySaveFeedbackComponent.SetOpenDecompilationProgress(2, 7);
    int thirdSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndString, secondSeparator);
    mySaveFeedbackComponent.SetOpenDecompilationProgress(3, 7);
    int fourthSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndEnum, thirdSeparator);
    mySaveFeedbackComponent.SetOpenDecompilationProgress(4, 7);
    int fithSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndTemplate, fourthSeparator);
    mySaveFeedbackComponent.SetOpenDecompilationProgress(5, 7);
    int sixthSeparator = WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndData, fithSeparator);
    mySaveFeedbackComponent.SetOpenDecompilationProgress(6, 7);
    WriteTempFileOnOpen(uncompressedData, tempFolderPath + fileEndPro, sixthSeparator);



    // III. Project Info

    ProcessProjTempFile(tempFolderPath, dbManager);


    // IV. Languages

    ProcessLangTempFile(tempFolderPath, dbManager);


    // V. String Table

    ProcessStringTempFile(tempFolderPath, dbManager);


    // VI. Enums

    ProcessEnumTempFile(tempFolderPath, dbManager);


    // VII. Struct Templ

    ProcessTemplTempFile(tempFolderPath, dbManager);


    // VIII. Fill Data except refs

    ProcessDataTempFile(tempFolderPath, dbManager);



    // IX. Clean Up

    QDir tempDir(tempFolderPath);
    tempDir.removeRecursively();
    myIsOpening = false;

}


void SaveManager::ProcessProjTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager)
{
    QFile projectFile(_tempFolderPath + fileEndPro);
    bool openProCheck = projectFile.open(QIODevice::ReadOnly);
    Q_ASSERT(openProCheck);
    QTextStream proIn(&projectFile);
    mySaveFeedbackComponent.SetOpenProjectProgress();

    QString proFirstLine = proIn.readLine();
    Q_ASSERT(proFirstLine == "###PROJECT_FOLDER###");
    _dbManager.SetAttributePrefix(proIn.readLine());
    _dbManager.SetAttributeSuffix(proIn.readLine());
    const bool autoSaveFile = proIn.readLine() == '1';
    _dbManager.SetAutoSave(autoSaveFile, proIn.readLine().toInt());
    _dbManager.SetProjectContentFolderPath(proIn.readLine());
    for (const auto& assetType : AttributeTypeHelper::assetTypes)
    {
        QString regexLine = proIn.readLine();
        _dbManager.SetAAssetRegex(assetType, regexLine.isEmpty() ? "*" : regexLine);
    }

    Q_ASSERT(proIn.atEnd());
    projectFile.close();
}
void SaveManager::ProcessLangTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager)
{
    QFile langFile(_tempFolderPath + fileEndLang);
    bool openLangCheck = langFile.open(QIODevice::ReadOnly);
    Q_ASSERT(openLangCheck);
    QTextStream langIn(&langFile);
    Q_ASSERT(DB_Manager::GetLanguagesCount() == 1); // only the default english language expected
    mySaveFeedbackComponent.SetOpenLanguageProgress();

    QString currentLine;
    while (!langIn.atEnd())
    {
        currentLine = langIn.readLine();
        int separatorIndex = currentLine.indexOf(" - ");
        Q_ASSERT(separatorIndex > 0);
        QString abbrev = currentLine.first(separatorIndex);
        QString name = currentLine.mid(separatorIndex + 3);

        Language language {abbrev, name};
        _dbManager.AddLanguage(language);
    }
    langFile.close();

    // Remove the default english language
    if (DB_Manager::GetLanguagesCount() > 0)
    {
        _dbManager.RemoveLanguage(0);
    }

    // Check for duplicates
    if (_dbManager.GetLanguages().HasDoubles())
    {
        qFatal() << "Save file led to duplicates languages which isn't allowed";
    }
}
void SaveManager::ProcessStringTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager)
{
    QMap<QString, SStringImporter> importerMap;

    QFile file(_tempFolderPath + fileEndString);
    bool openCheck = file.open(QIODevice::ReadOnly);
    Q_ASSERT(openCheck);
    QTextStream in(&file);

    const auto& languages = DB_Manager::GetDB_Manager().GetLanguages();

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

            importerMap[currentTable].RegisterLanguageFile(languages.GetLanguageIndexFromAbbrev(currentLangg), currentFilePath);
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
    int i = 0;
    for (const auto& tableName : stringTableNames)
    {
        mySaveFeedbackComponent.SetOpenStringTableProgress(i, stringTableNames.length());
        importerMap[tableName].PerformImport(-2, 0, tableName); // -1 is for Dictionary import via DialogBox
        i++;
    }


    _dbManager.ReplaceDictionaryWithLastStringTable();  // TODO ? : directly import Dictionary with ....PerformImport(-1, 0, tableName); instead of using a temporary StringTable

    file.close();
}
void SaveManager::ProcessEnumTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager)
{
    QFile file(_tempFolderPath + fileEndEnum);
    bool openCheck = file.open(QIODevice::ReadOnly);
    Q_ASSERT(openCheck);
    QTextStream in(&file);
    mySaveFeedbackComponent.SetOpenEnumProgress();

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
    const int structCount = importedJson.count();

    // First loop to create the structures and set the "identity" values
    int i = 0;
    for (const QJsonValue& templAsJson : importedJson)
    {
        mySaveFeedbackComponent.SetOpenStructTemplateProgress(i, structCount);
        _dbManager.AddStructureDB(TemplateStructure::LoadTemplateNoAttribute(templAsJson.toObject()));
        i++;
    }

    // Second loop to set the attributes (parameters and default values)
    i = 0;
    for (const QJsonValue& templAsJson : importedJson)
    {
        mySaveFeedbackComponent.SetOpenStructTemplateDefaultProgress(i, structCount);
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

    int i = 0;
    QStringList structNames = importedJson.keys();
    for (const auto& strctName : structNames)
    {
        mySaveFeedbackComponent.SetOpenStructDataProgress(i, structNames.length());
        StructureDB* currentStructTable = _dbManager.GetStructureTable(strctName);
        Q_ASSERT(currentStructTable != nullptr);
        currentStructTable->ReadValue_JSON_Table(importedJson.value(strctName).toArray(), StructureImportHelper::OverwritePolicy::Overwrite);
        i++;
    }
}


bool SaveManager::GetHasUnsavedChanges()
{
    return GetSaveManager().myHasUnsavedChanges;
}
void SaveManager::AcknowledgeUnsavedChanges()
{
    GetSaveManager().myHasUnsavedChanges = true;
    DB_Manager::GetDB_Manager().NotifyUnsavedChanges();
}


