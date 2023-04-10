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
    void SaveFileInternal(const QString& _saveFilePath);
    void OpenFileInternal(const QString& _saveFilePath);

public:
    static void SaveFile(const QString& _saveFilePath);
    static void OpenFile(const QString& _saveFilePath);
};

#endif // SAVEMANAGER_H
