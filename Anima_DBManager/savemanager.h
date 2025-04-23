#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QMap>
#include "areference.h"
#include "db_manager.h"

class SaveManager
{
private:
    SaveManager();
    SaveManager(const SaveManager&) = delete;
    ~SaveManager();

    static const QByteArray separator;
    static const QString fileEndString;
    static const QString fileEndEnum;
    static const QString fileEndTemplate;
    static const QString fileEndData;
    static const QString fileEndPro;

    bool myIsOpening = false;
    bool myIsSaving = false;
    QString myCurrentlyOpenedFile = "";


    static SaveManager& GetSaveManager();
    static QString GetSaveFileTempFolder(const QString& _saveFilePath);
    static bool TryMakeTempFolder(const QString& _tempFolderPath);
    static int FindFileSeparatorStart(const QByteArray& _data, const int _start);
    static int WriteTempFileOnOpen(const QByteArray& _data, const QString& _tempFilePath, const int _separatorBegin);
    void SaveAutoInternal();
    void SaveFileInternal(const QString& _saveFilePath, bool _isAutoSave = false);
    void OpenFileInternal(const QString& _saveFilePath);
    void SetCurrentlyOpenedFile(const QString& _file);

    void ProcessProjTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager);
    void ProcessStringTempFile(const QString& _tempFolderPath);
    void ProcessEnumTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager);
    void ProcessTemplTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager);
    void ProcessDataTempFile(const QString& _tempFolderPath, DB_Manager& _dbManager);

public:
    static bool HasCurrentFile();
    static const QString& GetCurrentSaveFile();
    static const QString& GetSaveFileExtension();
    static void New();
    static void SaveAuto();
    static void SaveFile(const QString& _saveFilePath);
    static void OpenFile(const QString& _saveFilePath);
    static bool IsOpeningFile();
    static bool IsSavingFile();
};

#endif // SAVEMANAGER_H
