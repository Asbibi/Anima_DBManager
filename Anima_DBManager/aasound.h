#ifndef AASOUND_H
#define AASOUND_H

#include "aasset.h"

class AASound : public AAsset
{
public:
    AASound(TemplateAttribute& _template);
    AASound(TemplateAttribute& _template, QString _filePath);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Sound; }
    virtual const QString& GetAssetClassNameForCSV()            const override;
    virtual const QString& GetAssetFileExtension()              const override;
    static const QString& GetStaticAssetFileExtension();
};

#endif // AASOUND_H
