#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QMap>
#include "areference.h"

class SaveManager
{
private:
    SaveManager();
    SaveManager(const SaveManager&) = delete;
    ~SaveManager();

    QMap<AReference, QString> myDelayedReferenceValues;

    static SaveManager& GetSaveManager();
    static QString GetSaveFileTempFolder(const QString& _saveFilePath);
    void SaveFileInternal(const QString& _saveFilePath);
    void OpenFileInternal(const QString& _saveFilePath);

public:
    static const QString& GetSaveFileExtension();
    static void SaveFile(const QString& _saveFilePath);
    static void OpenFile(const QString& _saveFilePath);
};

#endif // SAVEMANAGER_H