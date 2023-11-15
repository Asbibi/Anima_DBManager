#ifndef AASSET_H
#define AASSET_H

#include "attribute.h"

class AAsset : public Attribute
{
protected:
    QString myFilePath;
    bool myIsDirty = false;
    void SetValueFromText(const QString& text);

public:
    static const QString noPathValue;

    AAsset(TemplateAttribute& _template);
    AAsset(TemplateAttribute& _template, QString _filePath);

    bool IsDirty() const;
    bool IsEmpty() const;
    const QString& GetFilePath() const;
    bool UpdateIsDirty();   // return if myIsDirty has changed

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::UAsset; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValue_CSV()                              const override;
    virtual QJsonValue GetValue_JSON()                          const override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)              override;
    virtual void SetValue_CSV(const QString& text)                    override;

    virtual const QString& GetAssetClassNameForCSV() const;
    virtual const QString& GetAssetFileExtension() const;
    static const QString& GetStaticAssetFileExtension();

    static QString GetFilePathForDisplay(const QString& _filePath, bool _isDirty);
    static bool IsDirty(const QString& _filePath);
};

#endif // AASSET_H
