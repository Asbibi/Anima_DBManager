#ifndef AASSET_H
#define AASSET_H

#include "attribute.h"

class AAsset : public Attribute
{
protected:
    QString myFilePath;
    bool myIsDirty = false;

public:
    AAsset(const AttributeParam& _sharedParam);
    AAsset(const AttributeParam& _sharedParam, QString _filePath);

    bool IsDirty() const;
    bool IsEmpty() const;
    const QString& GetFilePath() const;
    bool UpdateIsDirty();   // return if myIsDirty has changed

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::UAsset; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual void ReadValue_CSV(const QString& text)                   override;

    virtual const QString& GetAssetClassNameForCSV() const;
    virtual const QString& GetAssetFileExtension() const;
    static const QString& GetStaticAssetFileExtension();

    static QString GetFilePathForDisplay(const QString& _filePath, bool _isDirty);
    static bool IsDirty(const QString& _filePath);
};

#endif // AASSET_H
