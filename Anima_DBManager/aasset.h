#ifndef AASSET_H
#define AASSET_H

#include "attribute.h"

class AAsset : public Attribute
{
protected:
    QString filePath;

public:
    AAsset(const AttributeParam& _sharedParam);
    AAsset(const AttributeParam& _sharedParam, QString _filePath);

    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;

    static QString GetFilePathForDisplay(const QString& _filePath);
};

#endif // AASSET_H
